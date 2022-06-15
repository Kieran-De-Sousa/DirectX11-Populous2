// Created by Ben 12/03/2022

#include "pch.h"

#include "Transform.h"
#include "Unit.h"
#include "Settle.h"
#include "Energy.h"
#include "Settlement.h"
#include "Emitter.h"
#include "Particle.h"
#include "Collider.h"
#include "SpriteRenderer.h"

#include "SettleSystem.h"
#include <iostream>

#include "Driver.h"

Engine::SettleSystem::SettleSystem() : coordinator(nullptr), tile_map(nullptr), path_finder(nullptr)
{
	// Can't initialise coordinator in here because of header only circular dependency issues
}

void Engine::SettleSystem::init(Coordinator* _coordinator, TileMap* _tile_map, PathFinder* _path_finder, RenderSystem* _renderer)
{
	coordinator = _coordinator;
	tile_map = _tile_map;
	path_finder = _path_finder;
	render_system = _renderer;
}

void Engine::SettleSystem::update(const DX::StepTimer& timer)
{
	for (const Entity& entity : entities)
	{
		Unit& entity_unit = coordinator->getComponent<Unit>(entity);

		Settle& settle = coordinator->getComponent<Settle>(entity);
		//Energy& energy = coordinator->getComponent<Energy>(entity);
		settle.timer += timer.GetElapsedSeconds();

		// Settler is trying to settle
		if (settle.timer > settle.check_placement_at)
		{
			Unit& unit = coordinator->getComponent<Unit>(entity);

			// Settler has enough energy to settle
			if (unit.energy > settle.min_energy)
			{
				// Find tile settler is on 
				Transform& transform = coordinator->getComponent<Transform>(entity);
				Engine::Vector2Int current_position = tile_map->worldToTile(transform.position);

				if (current_position.x >= tile_map->getWidth())
				{
					if (current_position.x > tile_map->getWidth() * 2)
					{
						current_position.x = 0;
					}
					else
					{
						current_position.x = tile_map->getWidth() - 1;
					}
				}

				if (current_position.y >= tile_map->getHeight())
				{
					if (current_position.y > tile_map->getHeight() * 2)
					{
						current_position.y = 0;
					}
					else
					{
						current_position.y = tile_map->getHeight() - 1;
					}
				}

				Engine::TileMap::Tile* current_tile = tile_map->findTile(current_position.x, current_position.y);

				std::cout << entity <<" is attempting settling at: " << current_position.x << ", " << current_position.y << std::endl;

				
				/*if (MAPPER.BuildableData.find(current_tile->tile_state).second)
				{

				}*/

				// See if tile can be settled on
				if (complexSettleCheck(current_position.x, current_position.y))
				{
					// Set tile to blocked or ruins
					current_tile->tile_state = TileMap::Tile::TileState::NonWalkable;

					// Create Settlement entity
					Entity settlement_entity = coordinator->createEntity();
					Settlement& settlement_data = coordinator->addComponent<Settlement>(settlement_entity, {});
					settlement_data.settlement_type = Engine::BuildingTypes::HUT;
					settlement_data.building_team = unit.unit_team;
					settlement_data.energy_per_sec = MAPPER.BuildingGeneration[Engine::BuildingTypes::HUT];
					Transform& settlement_transform = coordinator->addComponent<Transform>(settlement_entity, {});
					//DirectX::SimpleMath::Vector3 building_offset = {18,0,0};
					settlement_transform.position = tile_map->tileToWorld(current_position);
					Energy& settlement_energy = coordinator->addComponent<Energy>(settlement_entity, {});
					settlement_energy.amount = unit.energy - settle.min_energy;
					SpriteRenderer& settlement_sprite = coordinator->addComponent<SpriteRenderer>(settlement_entity, {});
					render_system->initSprite(Engine::Driver::getLoadHandler()->getResourcePath(
						entity_unit.unit_team == Engine::Teams::RED ? Engine::LoadHandler::Resources::HOUSE_TEXTURE_R : Engine::LoadHandler::Resources::HOUSE_TEXTURE_B),
						settlement_entity);
					settlement_transform.position = settlement_transform.position/* - height_offset*/;
					Collider& settlement_collider = coordinator->addComponent<Collider>(settlement_entity, {});
					settlement_collider.collider_size = {settlement_sprite.texture_dimensions.x,settlement_sprite.texture_dimensions.y };
					settlement_collider.on_collision_enter = [this](const Engine::Entity& t, const Engine::Entity& v) {onCollisionEnter(t, v);  };
					settlement_collider.on_collision_exit = [this](const Engine::Entity& t, const Engine::Entity& v) {onCollisionExit(t, v);  };
					//cout << "Tile offset = " << height_offset.y << endl;

					// Destroy Settler
					coordinator->destroyEntityAtEndOfFrame(entity);
					std::cout << "Settling Succeeded at: " << current_position.x << ", " << current_position.y << std::endl;
					std::cout << "New entity: " << settlement_entity << std::endl;
				}
				else
				{
					std::cout << "Settling failed at: " << current_position.x << ", " << current_position.y << std::endl;
					settle.delay_check = settle.delay_min + (settle.delay_random * (float)rand() / ((float)RAND_MAX + 1));
					std::cout << entity << " has a new Settling delay of " << settle.delay_check << std::endl;
					settle.check_placement_at = settle.delay_check;
					settle.timer = 0.0F;
				}

				//tile_map->findTile(current_position.x, current_position.y)->tile_state = TileMap::Tile::TileState::NonWalkable;
				// So to change the tile map tile sprite, on the sprites creation a reference to its sprite will need to get
				// assigned to the tile. Then we can have a function can't like updateTileState which will take in a tile and a
				// state (walkable, not walkable, etc) and can then update the relevant values 

				//path_finder->findNode(current_position.x, current_position.y)->obstacle = true;

				// std::cout << "placed house at: " << current_position.x << "," << current_position.y << std::endl;

				//settle.house_place_timer = 0;
				//unit.energy -= settle.energy_cost;
			}
		}
	}
}

bool Engine::SettleSystem::simpleSettleCheck(int x, int y)
{
	return onTheGrid(x, y) && tile_map->findTile(x,y)->tile_state == Engine::TileMap::Tile::TileState::Grass;
}

bool Engine::SettleSystem::complexSettleCheck(int x, int y)
{
	bool answer = true;
	if (onTheGrid(x,y)&&tile_map->findTile(x, y)->tile_state == Engine::TileMap::Tile::TileState::Grass)
	{
		answer = false;
	}

	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			if (onTheGrid(x+i, y+j) && tile_map->findTile(x + i, y + j)->tile_state == Engine::TileMap::Tile::TileState::NonWalkable)
			{
				answer = false;
			}
		}
	}

	return answer;
}

bool Engine::SettleSystem::onTheGrid(int x, int y)
{
	return x>=0 && y>=0 && x<tile_map->getWidth() && y<tile_map->getHeight();
}

void Engine::SettleSystem::onCollisionEnter(const Engine::Entity& t, const Engine::Entity& v)
{
	if (coordinator->hasComponent<Engine::Settlement>(t) && coordinator->hasComponent<Engine::Particle>(v))
	{
		Engine::Particle& particle = coordinator->getComponent<Engine::Particle>(v);
		if (particle.damaging)
		{
			coordinator->destroyEntityAtEndOfFrame(t);
		}
	}

	if (coordinator->hasComponent<Engine::Settlement>(t) && coordinator->hasComponent<Engine::Emitter>(v))
	{
		Engine::Emitter& emitter = coordinator->getComponent<Engine::Emitter>(v);
		if (emitter.damage > 0.0F)
		{
			coordinator->destroyEntityAtEndOfFrame(t);
		}
	}
}

void Engine::SettleSystem::onCollisionExit(const Engine::Entity& t, const Engine::Entity& v)
{

}
