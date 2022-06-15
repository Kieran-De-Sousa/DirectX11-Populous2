// Created by Ben on 07/03/2022

#pragma once

// DirectX
#include "StepTimer.h"

// Project
#include "TileMap.h"
#include "PathFinder.h"
#include "Unit.h"
#include "System.h"
#include "Coordinator.h"
#include "CollisionSystem.h"
#include "Transform.h"
#include "RigidBody.h"
#include "Transform.h"

namespace Engine
{
	/// <summary>
	/// Handles things that all units, regardless of type, need or do
	/// </summary>
	class UnitSystem : public Engine::System
	{
	public:
		UnitSystem();
		~UnitSystem() = default;

		/// <summary>
		/// Init the system, systems can't have constructors so required after creation
		/// </summary>
		/// <param name="_coordinator"> | Pointer to the scene's coordinator </param>
		/// <param name="_tile_system"> | Pointer to the scene's tilemap </param>
		/// <param name="_path_finder"> | Pointer to the scene's pathfinder </param>
		/// <param name="_collision_system"> | Pointer to the scene's collision system</param>
		/// <param name="_render_system"> | Pointer to the scene's render system</param>
		void init(Coordinator* _coordinator, TileMap* _tile_system, PathFinder* _path_finder,
			      const std::shared_ptr<CollisionSystem>& _collision_system, const std::shared_ptr<RenderSystem>& _render_system);
		/// <summary>
		/// Updates the system, handling pathfinding for units
		/// </summary>
		/// <param name="timer">Driver's->Scene's->StepTimer</param>
		void update(const DX::StepTimer& timer);

		/// <summary>
		/// Creates an agent 
		/// </summary>
		/// <param name="entity"> | The entity whos components to init </param>
		/// <param name="start_transform_values"> | The initial values the entitys transform component </param>
		/// <param name="start_rigidbody_values"> | The intial values of the entitys rigidbody component </param>
		/// <param name="start_unit_values"> | The intial values of the entities unit compoonent </param>
		/// <param name="texture_path"> | The units texture path </param>
		/// <param name="_on_collision_enter"> | The units onCollisionEnter method </param>
		/// <param name="_on_collision_exit"> | The units onCollisionExit method </param>
		/// <param name="collider_size"> | The size of the units collider </param>
		/// <param name="ai_speed"> | How fast the npc will move </param>
		/// <param name="ai_sensitivity"> | How sensitive the ai is (how close it has to get to the desitation before it turns around) </param>
		void initAgent(const Entity& entity,                                                                                                                                                                                                                         // Entity 
			           const Transform& start_transform_values, const RigidBody& start_rigidbody_values, const Unit& start_unit_values,                                                                                                                              // Components 
			           const std::string& texture_path,                                                                                                                                                                                                              // Sprite Renderer 
			           const std::function<void(const Engine::Entity&, const Engine::Entity&)>& _on_collision_enter, const std::function<void(const Engine::Entity&, const Engine::Entity&)>& _on_collision_exit, const DirectX::SimpleMath::Vector2& collider_size, // Collider
			           float ai_speed, float ai_sensitivity);                                                                                                                                                                                                        // Pathfinder

		/// <summary>
		/// Moves all agents of the specified team towards the specified point
		/// </summary>
		/// <param name="affected_unit_team"> | Which team the move towards point will effect </param>
		/// <param name="point"> | The point the units should move towards </param>
		void moveTowardsPoint(Engine::Teams affected_unit_team, const DirectX::SimpleMath::Vector3& point);
		/// <summary>
		/// Moves all agents of the specified team towards the specific point in a range 
		/// </summary>
		/// <param name="point"> | The point to which the units will move towards </param>
		/// <param name="range"> | How close the units have to be to the point to be effected </param>
		/// <param name="affected_unit_team"> | Which units team will be effected </param>
		void moveTowardsPointInRange(const DirectX::SimpleMath::Vector3& point, float range, Engine::Teams affected_unit_team);
		/// <summary>
		/// Combine two npcs together
		/// </summary>
		/// <param name="unit_one"> | The first unit involved in operation</param>
		/// <param name="unit_two"> | The second unit involed in the operation</param>
		void merge(const Entity& unit_one, const Entity& unit_two);
		/// <summary>
		/// Make two npcs fight
		/// </summary>
		/// <param name="unit_one"> | The first unit involved in the operation </param>
		/// <param name="unit_two"> | The second unit involved in the operation </param>
		void fight(const Entity& unit_one, const Entity& unit_two);
		
	private:
		/// <summary>
		/// Picks a random point in a grid and calculates the shortest path
		/// </summary>
		/// <param name="entity">| The entity to effect</param>
		void calculatePath(const Entity& entity);
		/// <summary>
		/// Helper method for converting a tile cord path to a vector one
		/// </summary>
		/// <param name="node_path"> | The pathfinder node path </param>
		/// <returns></returns>
		std::deque<DirectX::SimpleMath::Vector3> nodeToVectorPath(const std::deque<PathFinder::Node*>& node_path);
		/// <summary>
		/// Moves the unit towards the end location in the path
		/// </summary>
		/// <param name="entity"> | The entity to effected </param>
		void moveUnitTowardsNode(const Entity& entity);

		/// <summary>
		/// Sorts npcs by their index so that the pair is the same when we compair them (because the compair will happen twice)
		/// </summary>
		/// <param name="unit_one"> | The first unit to be sorted </param>
		/// <param name="unit_two"> | The second unit to be sorted </param>
		/// <returns> The sorted entity pair </returns>
		std::pair<Entity, Entity> sortUnitsForInteractions(const Entity& unit_one, const Entity& unit_two);

		/// <summary>
		/// Pointer to the scene's coordinator
		/// </summary>
		Coordinator* coordinator;
		/// <summary>
		/// Pointer to the scene's tilemap
		/// </summary>
		TileMap* tile_map;      // Not ideal but can't think of a better way to get the data we need right now
		/// <summary>
		/// Pointer to the scene's tilemap
		/// </summary>
		PathFinder* path_finder; // ^^

		/// <summary>
		/// Pointer to the scene's collision system
		/// </summary>
		std::shared_ptr<CollisionSystem> collision_system; // ^
		/// <summary>
		/// Pointer to the scene's render system
		/// </summary>
		std::shared_ptr<RenderSystem>    render_system;    //   ^

		/// <summary>
		/// A map of entity - entity storing all enemies that are currently merging
		/// </summary>
		std::unordered_map<Entity, Entity> merge_map;
		/// <summary>
		/// A map of entity - entity storing all entitiing that are currently fighting 
		/// </summary>
		std::unordered_map<Entity, Entity> fight_map;

		/// <summary>
		/// The names for the unit animations
		/// </summary>
		const std::string UP_ANIMATION    = "UP";
		const std::string DOWN_ANIMATION  = "DOWN";
		const std::string LEFT_ANIMATION  = "LEFT";
		const std::string RIGHT_ANIMATION = "RIGHT";
	};
} // namespace Engine

/*
	float screen_width  = std::rand() % 800;
	float screen_height = std::rand() % 600;

	coordinator->getComponent<AI>(entity).path.push_back(DirectX::SimpleMath::Vector3(screen_width, screen_height, 0));
	return;
*/
