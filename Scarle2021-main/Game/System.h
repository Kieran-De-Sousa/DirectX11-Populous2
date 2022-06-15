// Created by Ben on 21/02/2022
// Resources: https://youtu.be/rX0ItVEVjHc | https://austinmorlan.com/posts/entity_component_system/ 

#pragma once

// System
#include <set>

// Project
#include "Types.h"

namespace Engine
{
	/// <summary>
	/// Base class of functionality for storing a list of entities.
	/// Each system can inherit from this class and keep a list of pointers to system.
	/// (Virtual inheritance / last minuite branching / overly generic functions = slower) 
	/// However, we do use templates/stl because its a lot more simple and I haven't accended to that level of wizardry
	/// </summary>
	class System
	{
	public:
		System() = default;
		~System() = default;

		/// <summary>
		/// All the entities in the system (set by the system's signature)
		/// </summary>
		std::set<Entity> entities;
	};
} // namespace Engine

/* Could do something like:
* 
	for (auto const& entity : entities)
	{
		auto& rigidBody = GetComponent<RigidBody>(entity);
		auto& transform = GetComponent<Transform>(entity);
		auto const& gravity = GetComponent<Gravity>(entity);

		transform.position += rigidBody.velocity * dt;

		rigidBody.velocity += gravity.force * dt;
	}

	A RigidBody, a Transform, and a Gravity will be pulled into the cache 
	for this entity as well as for all of the entities near it in the component array, 
	which are likely to be needed with the next entity in the list of entities.
*/
