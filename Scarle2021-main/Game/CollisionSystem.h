// Created by Ben 15/03/2022

#pragma once

#include "TileMap.h"

#include "System.h"
#include "Coordinator.h"

namespace Engine
{
	/// <summary>
	/// Calculates collision based off AABB
	/// </summary>
	class CollisionSystem : public Engine::System
	{
	public:
		CollisionSystem();
		~CollisionSystem() = default;

		/// <summary>
		/// Init the system, systems can't have constructors so required after creation
		/// </summary>
		void init(Coordinator* _coordinator);
		/// <summary>
		/// Update the system, checking AABB colliisons 
		/// </summary>
		/// <param name="timer"> Driver->Scene's StepTimer</param>
		void update(const DX::StepTimer& timer); 

		/// <summary>
		/// Init a collider with the supplied values 
		/// </summary>
		/// <param name="_on_collision_enter"> | The function which is called if the entity enters a collision </param>
		/// <param name="_on_collision_exit"> | The function which is called if the entity exits a collision </param>
		/// <param name="collider_size"> | The size of the collider </param>
		/// <param name="entity"> | The entity collider to init </param>
		void initCollider(const std::function<void(const Engine::Entity&, const Engine::Entity&)>& _on_collision_enter, const std::function<void(const Engine::Entity&, const Engine::Entity&)>& _on_collision_exit,
			              const DirectX::SimpleMath::Vector2& collider_size, const Engine::Entity& entity);

		/// [Deprecated]
		void deinitColliderAtEndOfFrame(const Engine::Entity& entity, bool delete_entity_as_well = false);
		/// [Deprecated]
		void deinitCollider(const Engine::Entity& entity);

		/// <summary>
		/// Detects all of the collisions in an area
		/// </summary>
		/// <param name="point"> | The point to base the area check around </param>
		/// <param name="size"> | The size of the area check </param>
		/// <param name="predicted_collisions"> | How many collisions could happen, reserves space in the returned vector </param>
		/// <returns> A vector of all the entities collided with </returns>
		std::vector<Engine::Entity> checkCollisionsInArea(const DirectX::SimpleMath::Vector2& point, const DirectX::SimpleMath::Vector2& size, int predicted_collisions=0);

	private:
		/// <summary>
		/// Checks if two entity's sprites are overlapping
		/// </summary>
		/// <param name="a"> | Entity A </param>
		/// <param name="b"> | Entity B </param>
		/// <returns> If entity a and entity b are overlapping </returns>
		bool checkAABB(const Engine::Entity& a, const Engine::Entity& b);

		/// <summary>
		/// Reference to the scene's coordinator
		/// </summary>
		Coordinator* coordinator;
	};
}
