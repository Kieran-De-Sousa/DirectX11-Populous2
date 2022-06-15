// Created by Ben and Tom on 07/03/2022

#include "pch.h"

#include <array>
#include <unordered_map>

#include "Collider.h"
#include "Transform.h"
#include "RigidBody.h"
#include "PathFinderAgent.h"
#include "Settle.h"
#include "SpriteRenderer.h"
#include "EnergyDecay.h"
#include "Driver.h"

#include "UnitSystem.h"

Engine::UnitSystem::UnitSystem() : coordinator(nullptr), tile_map(nullptr), path_finder(nullptr), collision_system(nullptr), render_system(nullptr)
{
	// Can't initialise coordinator in here because of header only circular dependency issues
}

void Engine::UnitSystem::init(Coordinator* _coordinator, TileMap* _tile_map, PathFinder* _path_finder, 
	                          const std::shared_ptr<CollisionSystem>& _collision_system, const std::shared_ptr<RenderSystem>& _render_system)
{
	coordinator = _coordinator;
	tile_map = _tile_map;
	path_finder = _path_finder;

	collision_system = _collision_system;
	render_system = _render_system;

	merge_map.reserve(100);
	fight_map.reserve(100);
	// paused_units.reserve(100);
}

void Engine::UnitSystem::update(const DX::StepTimer& timer)
{
	for (const Entity& entity : entities)
	{
		// Action timer
		Unit& unit = coordinator->getComponent<Unit>(entity);
		unit.action_clock += timer.GetElapsedSeconds();

		// Pathfinding
		Transform& transform = coordinator->getComponent<Transform>(entity);
		PathFinderAgent& ai = coordinator->getComponent<PathFinderAgent>(entity);

		if (!ai.path.empty() && DirectX::SimpleMath::Vector3::Distance(transform.position, ai.path.back()) < ai.sensitivity)
		{
			ai.path.pop_back();
			if (ai.path.empty())
			{
				calculatePath(entity);
			}

			DirectX::SimpleMath::Vector3 velocity = coordinator->getComponent<Engine::RigidBody>(entity).velocity;
			velocity.Normalize();

			if (velocity.y > abs(velocity.x) && velocity.y >= 0)
			{
				//down
				render_system->changeAnimation(entity, DOWN_ANIMATION);
			}
			else if (abs(velocity.y) > abs(velocity.x) && velocity.y <= 0)
			{
				//up
				render_system->changeAnimation(entity, UP_ANIMATION);
			}
			else if (velocity.x > abs(velocity.y) && velocity.x >= 0)
			{
				//right
				render_system->changeAnimation(entity, RIGHT_ANIMATION);
			}
			else if (abs(velocity.x) > abs(velocity.y) && velocity.x <= 0)
			{
				//left
				render_system->changeAnimation(entity, LEFT_ANIMATION);
			}			
			moveUnitTowardsNode(entity);
		}
	}
}

void Engine::UnitSystem::initAgent(const Entity& entity, 
								   const Transform& start_transform_values, const RigidBody& start_rigidbody_values, const Unit& start_unit_values, 
	                               const std::string& texture_path, 
	                               const std::function<void(const Engine::Entity&, const Engine::Entity&)>& _on_collision_enter, const std::function<void(const Engine::Entity&, const Engine::Entity&)>& _on_collision_exit, const DirectX::SimpleMath::Vector2& collider_size, 
	                               float ai_speed, float ai_sensitivity)
{
	// Component Setup
	Transform& transform = coordinator->addComponent<Engine::Transform>(
		entity,
		start_transform_values); 
	RigidBody& rigid_body = coordinator->addComponent<Engine::RigidBody>(
		entity,
		start_rigidbody_values);
	SpriteRenderer& sprite_renderer = coordinator->addComponent<Engine::SpriteRenderer>(
	    entity, 
		{});
	PathFinderAgent& path_finder_agent = coordinator->addComponent<Engine::PathFinderAgent>(
		entity, 
		{});
	Unit& unit = coordinator->addComponent<Engine::Unit>(
		entity, 
		start_unit_values);
	//Testing:
	//unit.energy = (float)rand() / ((float)RAND_MAX + 1) * unit.max_energy;
	std::cout << entity <<": Spawned with " << unit.energy << " starting energy." << std::endl;
	//Testing End
	Settle& settle = coordinator->addComponent<Engine::Settle>(
		entity, 
		{});
	EnergyDecay& decay = coordinator->addComponent<Engine::EnergyDecay>(entity, {});
	//Testing:
	//decay.decay_per_sec = 0.2F;
	//Testing End
	Collider& collider = coordinator->addComponent<Engine::Collider>(
		entity, 
		{});
	collider.collider_size = collider_size;
	collider.on_collision_enter = _on_collision_enter;
	collider.on_collision_exit = _on_collision_exit;

	collision_system->initCollider(_on_collision_enter, _on_collision_exit, collider_size, entity);
	render_system->initSprite(texture_path, entity);

	// Animations
	std::array<LONG, 4> up_frame_one = { 0, 0, 72, 72 };
	std::array<LONG, 4> up_frame_two = { 144, 72, 216, 144 };
	std::vector<std::array<LONG, 4>> up_animation = { up_frame_one, up_frame_two };

	std::array<LONG, 4> down_frame_one = { 72, 72, 144, 144 };
	std::array<LONG, 4> down_frame_two = { 0, 216, 72, 288 };
	std::vector<std::array<LONG, 4>> down_animation = { down_frame_one, down_frame_two };

	std::array<LONG, 4> left_frame_one = { 72, 216, 144, 288};
	std::array<LONG, 4> left_frame_two = { 144, 216, 216, 288};
	std::vector<std::array<LONG, 4>> left_animation = { left_frame_one, left_frame_two };

	std::array<LONG, 4> right_frame_one = {144, 0, 216, 72};
	std::array<LONG, 4> right_frame_two = {72, 144, 144, 216};
	std::vector<std::array<LONG, 4>> right_animation = { right_frame_one, right_frame_one };

	std::unordered_map<std::string, std::vector<std::array<LONG, 4>>> animation_frames = {
		{ UP_ANIMATION,    up_animation },
		{ DOWN_ANIMATION,  down_animation }, 
		{ LEFT_ANIMATION,  left_animation }, 
		{ RIGHT_ANIMATION, right_animation }
	};

	//goblin animations
	//std::array<LONG, 4> up_frame_one = { 0, 0, 72, 72 };
	//std::array<LONG, 4> up_frame_two = { 144, 72, 216, 144 };
	//std::vector<std::array<LONG, 4>> up_animation = { up_frame_one, up_frame_two };

	//std::array<LONG, 4> down_frame_one = { 72, 72, 144, 144 };
	//std::array<LONG, 4> down_frame_two = { 0, 216, 72, 288 };
	//std::vector<std::array<LONG, 4>> down_animation = { down_frame_one, down_frame_two };

	//std::array<LONG, 4> left_frame_one = { 72, 216, 144, 288};
	//std::array<LONG, 4> left_frame_two = { 144, 216, 216, 288};
	//std::vector<std::array<LONG, 4>> left_animation = { left_frame_one, left_frame_two };

	//std::array<LONG, 4> right_frame_one = {144, 0, 216, 72};
	//std::array<LONG, 4> right_frame_two = {72, 144, 144, 216};
	//std::vector<std::array<LONG, 4>> right_animation = { right_frame_one, right_frame_one };

	//std::unordered_map<std::string, std::vector<std::array<LONG, 4>>> animation_frames = {
	//	{ UP_ANIMATION,    up_animation },
	//	{ DOWN_ANIMATION,  down_animation }, 
	//	{ LEFT_ANIMATION,  left_animation }, 
	//	{ RIGHT_ANIMATION, right_animation }
	//};

	render_system->initAnimator(entity, animation_frames, 0.12);

	// Agent Setup
	calculatePath(entity);
	DirectX::SimpleMath::Vector3 move_direction;
	if (!path_finder_agent.path.empty())
	{
		move_direction = path_finder_agent.path.back() - transform.position;
	}
	move_direction.Normalize();

	path_finder_agent.speed = ai_speed;
	path_finder_agent.sensitivity = ai_sensitivity;
	rigid_body.velocity = move_direction * path_finder_agent.speed;
}

void Engine::UnitSystem::moveTowardsPoint(Engine::Teams affected_unit_team, const DirectX::SimpleMath::Vector3& point)
{
	for (const Entity& entity : entities)
	{
		Unit& unit = coordinator->getComponent<Unit>(entity);
		//if (unit.unit_team != affected_unit_team) continue; <-- Comment this back in, was just for testing 

		PathFinderAgent& ai = coordinator->getComponent<PathFinderAgent>(entity);
		Transform& transform = coordinator->getComponent<Transform>(entity);

		// todo random offset so all npcs dont get crowd on each other?

		Engine::Vector2Int current_position = tile_map->worldToTile(transform.position);
		Engine::Vector2Int target_position = tile_map->worldToTile(point);

		std::deque<PathFinder::Node*> path = path_finder->findPath(current_position.x, current_position.y,
			                                                       target_position.x, target_position.y);

		if (path.empty()) return; // We all already on the position, don't need to do anything

		ai.path = nodeToVectorPath(path); //Don't understand why this doesn't work. Debug it soontm
		//ai.path = { point };

		moveUnitTowardsNode(entity);
	}
}

void Engine::UnitSystem::moveTowardsPointInRange(const DirectX::SimpleMath::Vector3& point, float range, Engine::Teams affected_unit_team)
{
	for (const Entity& entity : entities)
	{
		Unit& unit = coordinator->getComponent<Unit>(entity);
		if (unit.unit_team != affected_unit_team) continue;

		Transform& transform = coordinator->getComponent<Transform>(entity);
		float distance_to_point = DirectX::SimpleMath::Vector3::Distance(point, transform.position);

		if (distance_to_point <= range)
		{
			PathFinderAgent& agent = coordinator->getComponent<PathFinderAgent>(entity);
			agent.path = { point }; // probs will have the same problem as moveTowardsPoint so change this when thats fixed

			moveUnitTowardsNode(entity);
		}
	}
}

void Engine::UnitSystem::merge(const Entity& unit_one, const Entity& unit_two)
{
	std::pair<Entity, Entity> unit_pair = sortUnitsForInteractions(unit_one, unit_two);
	
	// Check if the merge is valud
	Unit& unit_one_unit = coordinator->getComponent<Unit>(unit_pair.first);
	Unit& unit_two_unit = coordinator->getComponent<Unit>(unit_pair.second);

	if (unit_one_unit.unit_team != unit_two_unit.unit_team ||
		unit_one_unit.action_clock < unit_one_unit.action_delay || unit_two_unit.action_clock < unit_two_unit.action_delay)
		// Can have more merge checks here. Eg: Type check
	{
		return;
	}

	// If the merge is valid, check if one unit has already signalled they are ready 
	if (merge_map.find(unit_pair.first) == merge_map.end())
	{
		merge_map.insert(unit_pair);
		return;
	}

	// Make sure units can't take part in any more actions 
	unit_one_unit.action_clock = -INFINITY;
	unit_two_unit.action_clock = -INFINITY;

	// Create new entity 
	float new_entity_energy      = unit_one_unit.energy + unit_two_unit.energy;
	Unit::UnitType new_unit_type = Unit::UnitType::Normal; // Some cool calculation here
	Engine::Teams new_unit_team = unit_one_unit.unit_team;

	Engine::Entity entity        = coordinator->createEntity();
	Transform& transform         = coordinator->getComponent<Transform>      (unit_pair.first);
	RigidBody& rigid_body        = coordinator->getComponent<RigidBody>      (unit_pair.first);
	Unit& unit                   = coordinator->getComponent<Unit>           (unit_pair.first);
	Collider& collider           = coordinator->getComponent<Collider>       (unit_pair.first);
	PathFinderAgent& agent       = coordinator->getComponent<PathFinderAgent>(unit_pair.first);

	initAgent(
		entity,                      // The entity 
		transform,                   // Transform 
		rigid_body,                  // Rigidbody
		unit,                        // Unit
		Engine::Driver::getLoadHandler()->getResourcePath(new_unit_team == Engine::Teams::RED ? Engine::LoadHandler::Resources::CHARACTER_TEXTURE :
		                                                                                                                                                                             Engine::LoadHandler::Resources::ENEMY_TEXTURE),                      // Sprite texture path
		collider.on_collision_enter, // On collision enter 
		collider.on_collision_exit,  // On collision exit
		collider.collider_size,      // Collider size
		agent.speed,                 // AI speed
		agent.sensitivity);          // AI sensitivity 
	
	// Delete old entity 
	coordinator->destroyEntityAtEndOfFrame(unit_one);
	coordinator->destroyEntityAtEndOfFrame(unit_two);

	merge_map.erase(unit_pair.first);

	// Debug message
	// std::cout << "New entity created!! New Entity Energy: " << new_entity_energy << std::endl;
}

void Engine::UnitSystem::fight(const Entity& unit_one, const Entity& unit_two)
{
	std::pair<const Entity&, const Entity&> unit_pair = sortUnitsForInteractions(unit_one, unit_two);

	// Check if the fight is valid
	Unit& unit_one_unit = coordinator->getComponent<Unit>(unit_pair.first);
	Unit& unit_two_unit = coordinator->getComponent<Unit>(unit_pair.second);

	if (unit_one_unit.unit_team == unit_two_unit.unit_team ||
		unit_one_unit.action_clock < unit_one_unit.action_delay || unit_two_unit.action_clock < unit_two_unit.action_delay)
		// Can have more merge checks here. Eg: Type check
	{
		return;
	}

	// If the fight is valid, check if one unit has already signalled they are ready 
	if (fight_map.find(unit_pair.first) == fight_map.end())
	{
		fight_map.insert(unit_pair);
		return;
	}

	// Make sure the units can't take part in any more actions
	unit_one_unit.action_clock = -INFINITY;
	unit_two_unit.action_clock = -INFINITY;

	// Work out the result of the fight
	if (unit_one_unit.energy == unit_two_unit.energy) 
	{
		bool coin_flip = static_cast<bool>(rand() % 2);
		unit_one_unit.energy += coin_flip ? 1 : -1;
	}

	if (unit_one_unit.energy > unit_two_unit.energy)
	{
		coordinator->destroyEntityAtEndOfFrame(unit_two);
		unit_one_unit.energy -= unit_two_unit.energy;
	}
	else
	{
		coordinator->destroyEntityAtEndOfFrame(unit_one);
		unit_two_unit.energy -= unit_one_unit.energy;
	}

	fight_map.erase(unit_pair.first);

	// Debug message
	// std::cout << "Two units fought!" << std::endl;
}

void Engine::UnitSystem::calculatePath(const Entity& entity)
{
	Transform& transform = coordinator->getComponent<Transform>(entity);
	PathFinderAgent& ai = coordinator->getComponent<PathFinderAgent>(entity);

	Engine::Vector2Int current_position = tile_map->worldToTile(transform.position);

	// some cool target operation soontm
	// Note - just make sure the new position is never the same coord as the old one!!!!! Otherwise nasty error

	std::deque<PathFinder::Node*> path;
	for (int i = 0; i < 5 && path.empty(); i++)// efficiency 101
	{
		PathFinder::Node* node = path_finder->findNode(rand() % tile_map->getWidth(), rand() % tile_map->getHeight());
		if (current_position.x >= tile_map->getWidth() || current_position.y >= tile_map->getHeight())
		{
			std::cout << "Entity " << entity << ": I'm dumb lol" << std::endl;
		}
		else
		{
			path = path_finder->findPath(current_position.x, current_position.y, node->x, node->y);
		}
	}

	ai.path = nodeToVectorPath(path);
}

std::deque<DirectX::SimpleMath::Vector3> Engine::UnitSystem::nodeToVectorPath(const std::deque<PathFinder::Node*>& node_path)
{
	std::deque<DirectX::SimpleMath::Vector3> vector_path;

	for (const auto& node : node_path)
	{
		// praise tom :pray:
		DirectX::SimpleMath::Vector3 cord = {0, 0, 0 };
		cord.x = tile_map->getMapOffset().x + (node->x - node->y) * tile_map->getTileSize().x / 2;
		cord.y = tile_map->getMapOffset().y + (node->x + node->y) * tile_map->getTileSize().y / 2;
		vector_path.push_back(cord);
	}
	return vector_path;
}

void Engine::UnitSystem::moveUnitTowardsNode(const Entity& entity)
{
	RigidBody& rigidbody = coordinator->getComponent<RigidBody>(entity);
	PathFinderAgent& agent = coordinator->getComponent<PathFinderAgent>(entity);
	Transform& transform = coordinator->getComponent<Transform>(entity);
	Unit& unit = coordinator->getComponent<Unit>(entity);

	if (!agent.path.empty())
	{
		DirectX::SimpleMath::Vector3 move_direction = agent.path.back() - transform.position;

		move_direction.Normalize();
		rigidbody.velocity = move_direction * agent.speed;
	}
	else
	{
		std::cout << "Whooops....\n";
	}
}

std::pair<Engine::Entity, Engine::Entity> Engine::UnitSystem::sortUnitsForInteractions(const Engine::Entity& unit_one, const Engine::Entity& unit_two)
{
	return { unit_one > unit_two ? unit_one, unit_two : unit_two, unit_one }; 
}
