#include "pch.h"
#include "AbilityAISystem.h"

#include <iostream>

Engine::AbilityAISystem::AbilityAISystem() : coordinator(nullptr)
{
	// Can't initialise coordinator in here because of header only circular dependency issues
}

void Engine::AbilityAISystem::init(Coordinator* _coordinator, TileMap* _tile_map, AbilityAIData _ai_data, std::shared_ptr<float> mana_pool, Engine::Teams _ai_team)
{
	coordinator = _coordinator;
	tile_map = _tile_map;
	ai_data = _ai_data;
	mana_ptr = mana_pool;
	ai_team = _ai_team;
}

void Engine::AbilityAISystem::update(const DX::StepTimer& timer)
{
	cast_timer -= timer.GetElapsedSeconds();
	//HAHA IT'S TIME TO WORK MY CHILD
	if (cast_timer <= 0.0F)
	{
		//Countup
		unsigned int own_unit = 0;
		unsigned int oth_unit = 0;
		unsigned int own_settlement = 0;
		unsigned int oth_settlement = 0;
		for (const auto& entity : entities)
		{
			if (coordinator->hasComponent<Settlement>(entity))
			{
				if (coordinator->getComponent<Settlement>(entity).building_team == ai_team)
				{
					own_settlement++;
				}
				else
				{
					oth_settlement++;
				}
			}

			if (coordinator->hasComponent<Unit>(entity))
			{
				if (coordinator->getComponent<Unit>(entity).unit_team == ai_team)
				{
					own_unit++;
				}
				else
				{
					oth_unit++;
				}
			}
		}
		//================================== ABILITY SELECTION ==============================================

		//Weight based ability selection
		float total_weight = 0.0F;
		AIAbilityInfo& chosen_ability = ai_data.list_of_abilities.at(0);
		for (auto& ability : ai_data.list_of_abilities)
		{
			//Calculate Ability Weight
			float ability_weight = ability.ab_weight_modifier;
			float ability_weight_multiplier = ability.ab_weight_multiplier;

			for (int i = 0; static_cast<float>(i) < *mana_ptr; i++)
			{
				ability_weight += ability.mana_modifier;
				ability_weight_multiplier *= ability.mana_multiplier;
			}

			for (int i = 0; i < own_unit; i++)
			{
				ability_weight += ability.ab_own_unit_mod;
				ability_weight_multiplier *= ability.ab_own_unit_mult;
			}

			for (int i = 0; i < oth_unit; i++)
			{
				ability_weight += ability.ab_oth_unit_mod;
				ability_weight_multiplier *= ability.ab_oth_unit_mult;
			}

			for (int i = 0; i < own_settlement; i++)
			{
				ability_weight += ability.ab_own_settlement_mod;
				ability_weight_multiplier *= ability.ab_own_settlement_mult;
			}

			for (int i = 0; i < oth_settlement; i++)
			{
				ability_weight += ability.ab_oth_settlement_mod;
				ability_weight_multiplier *= ability.ab_oth_settlement_mult;
			}

			ability_weight *= ability_weight_multiplier;
			if (ability.mana_cost * ai_data.mana_cost_mod > *mana_ptr)
			{
				ability_weight *= 0.0F;
			}

			//Add weight to pool and mark it
			total_weight += ability_weight;
			ability.ability_weight_marker = ability_weight;
		}
		//Now select ability from list
		if (total_weight > 0.0F)
		{
			bool only_act_once = true;
			float randomizer = ((float)rand() / ((float)RAND_MAX + 1)) * total_weight;
			for (auto& ability : ai_data.list_of_abilities)
			{
				randomizer -= ability.ability_weight_marker;
				if (only_act_once && randomizer <= 0.0F)
				{
					only_act_once = false;
					chosen_ability = ability;
				}
			}
			//===================================== TARGET SELECTION =====================================
			//Heatmap generation based on ability
			const int map_width = tile_map->getWidth();
			const int map_height = tile_map->getHeight();
			const int segments_x = map_width % chosen_ability.segmentation == 0 ? map_width / chosen_ability.segmentation
				                                                               : (map_width / chosen_ability.segmentation) + 1;
			const int segments_y = map_height % chosen_ability.segmentation == 0 ? map_height / chosen_ability.segmentation
																				: (map_height / chosen_ability.segmentation) + 1;
			const int segment_count = segments_x * segments_y;

			float* heat_map = new float[segment_count];
			float* heat_map_mult = new float[segment_count];
			//  std::make_unique<Node[]>(map_width * map_height); ssshhhhhh
			fill(heat_map, segment_count, 0.0F);
			fill(heat_map_mult, segment_count, 1.0F);

			for (const auto& entity : entities)
			{
				Engine::Vector2Int entity_coord = tile_map->worldToTile(coordinator->getComponent<Transform>(entity).position);
				if (coordinator->hasComponent<Unit>(entity))
				{
					if (coordinator->getComponent<Unit>(entity).unit_team == ai_team)
					{
						heat_map[entity_coord.x / chosen_ability.segmentation + segments_x * entity_coord.y / chosen_ability.segmentation] += chosen_ability.map_own_unit_mod;
						heat_map_mult[entity_coord.x / chosen_ability.segmentation + segments_x * entity_coord.y / chosen_ability.segmentation] *= chosen_ability.map_own_unit_mult;
					}
					else
					{
						heat_map[entity_coord.x / chosen_ability.segmentation + segments_x * entity_coord.y / chosen_ability.segmentation] += chosen_ability.map_oth_unit_mod;
						heat_map_mult[entity_coord.x / chosen_ability.segmentation + segments_x * entity_coord.y / chosen_ability.segmentation] *= chosen_ability.map_oth_unit_mult;
					}
				}
				if (coordinator->hasComponent<Settlement>(entity))
				{
					if (coordinator->getComponent<Settlement>(entity).building_team == ai_team)
					{
						heat_map[entity_coord.x / chosen_ability.segmentation + segments_x * entity_coord.y / chosen_ability.segmentation] += chosen_ability.map_own_settlement_mod;
						heat_map_mult[entity_coord.x / chosen_ability.segmentation + segments_x * entity_coord.y / chosen_ability.segmentation] *= chosen_ability.map_own_settlement_mult;
					}
					else
					{
						heat_map[entity_coord.x / chosen_ability.segmentation + segments_x * entity_coord.y / chosen_ability.segmentation] += chosen_ability.map_oth_settlement_mod;
						heat_map_mult[entity_coord.x / chosen_ability.segmentation + segments_x * entity_coord.y / chosen_ability.segmentation] *= chosen_ability.map_oth_settlement_mult;
					}
				}
			}
			//Count up total heat / tickets
			float total_tickets = 0.0F;
			int selected_segment = 0;
			bool selection_made = false;
			for (int i = 0; i < segment_count; i++)
			{
				if (heat_map[i] == 0.0F)
				{
					heat_map[i] = chosen_ability.empty_segment_ticket;
				}
				total_tickets += heat_map[i] * heat_map_mult[i];
			}

			float randomizer2 = ((float)rand() / ((float)RAND_MAX + 1)) * total_tickets;
			//Select the segment to aim at
			for (int i = 0; !selection_made && i < segment_count; i++) //avoiding while loops like the plague
			{
				randomizer2-= heat_map[i] * heat_map_mult[i];
				if (randomizer2 >= 0.0F)
				{
					selection_made = true;
					selected_segment = i;
				}
			}
			//Select a tile in the segment
			Engine::Vector2Int selected_tile = {(selected_segment%segments_x)*chosen_ability.segmentation,(selected_segment / segments_x) * chosen_ability.segmentation };
			selected_tile.x+=HelperMethods::MoreMath::randomRange(0, chosen_ability.segmentation - 1);
			selected_tile.y+=HelperMethods::MoreMath::randomRange(0, chosen_ability.segmentation - 1);
			//Offset the spell to be the centre of the tile
			DirectX::SimpleMath::Vector2 offset = tile_map->getTileSize();
			offset.x /= 2;
			offset.y /= 2;
			DirectX::SimpleMath::Vector3 ability_location = tile_map->tileToWorld(selected_tile);
			ability_location.x += offset.x;
			ability_location.y += offset.y;
			//Create an Ability Spawner entity for another system to use
			Entity ability_spawner = coordinator->createEntity();
			*mana_ptr -= chosen_ability.mana_cost * ai_data.mana_cost_mod;
			Transform& ability_spawner_transform = coordinator->addComponent<Transform>(ability_spawner, {});
			ability_spawner_transform.position = ability_location;
			AbilitySpawner& ability_spawner_component = coordinator->addComponent<AbilitySpawner>(ability_spawner, {});
			ability_spawner_component.ability = chosen_ability.ability;
			ability_spawner_component.delay = chosen_ability.cast_delay;
			ability_spawner_component.delay += chosen_ability.cast_delay_variability * ((float)rand() / ((float)RAND_MAX + 1));
			ability_spawner_component.source_team = ai_team;

			std::cout << "ALERT: AI USED AN ABILITY" << std::endl;

			delete heat_map;
			delete heat_map_mult;
		}
		//Add randomization to the casting
		cast_timer = ai_data.standard_wait;
		cast_timer += ((float)rand() / ((float)RAND_MAX + 1)) * ai_data.random_wait;
		if (*mana_ptr > ai_data.mana_mod_threshold)
		{
			cast_timer *= ai_data.mana_wait_mod;
		}
		//TODO: Add accuracy 
	}
	
}

void Engine::AbilityAISystem::addAbility(const AIAbilityInfo& new_ability)
{
	ai_data.list_of_abilities.push_back(new_ability);
}

void Engine::AbilityAISystem::clearAbilityList()
{
	ai_data.list_of_abilities.clear();
}

void Engine::AbilityAISystem::fill(float* t_array, unsigned length, float value)
{
	for (int i = 0; i < length; i++)
	{
		t_array[i] = value;
	}
}
