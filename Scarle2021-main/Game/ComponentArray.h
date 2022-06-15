// Created by Ben on 21/02/2022
// Resources: https://youtu.be/rX0ItVEVjHc | https://austinmorlan.com/posts/entity_component_system/ 

#pragma once

// System
#include <array>
#include <cassert>
#include <unordered_map>

// Program
#include "Types.h"

namespace Engine
{
	/// <summary>
	/// An interface is needed so that the ComponentManager  can tell a generic ComponentArray that 
	/// an entity has been destroyed and that it needs to update its array mappings.
	/// </summary>
	class IComponentArray
	{
	public:
		IComponentArray() = default;
		virtual ~IComponentArray() = default;

		virtual void entityDestroyed(Entity entity) = 0;
	};

	/// <summary>
	/// So this is basically an array of all the components on all the entities of type T
	/// Its packed array. The point of ECS is to keep things packed in memory, meaning you don't need any
	/// 'is valid' checks. Last second branches cause slowdowns!
	/// </summary>
	/// <typeparam name="T"> | Type of component </typeparam>
	template<typename T>
	class ComponentArray : public IComponentArray
	{
	public:
		ComponentArray() = default;
		~ComponentArray() = default;

		/// <summary>
		/// Adds a componet to an entity
		/// </summary>
		/// <param name="entity"> | The entity to add the componen to </param>
		/// <param name="component"> | The compone to tadd</param>
		void insertData(Entity entity, T component)
		{
			assert(entity_to_index_map.find(entity) == entity_to_index_map.end() &&
				"Component added to same entity more than once.");

			// Put new entry at end and update the maps
			size_t newIndex = size;
			entity_to_index_map[entity] = newIndex;
			index_to_entity_map[newIndex] = entity;
			component_array[newIndex] = component;
			++size;
		}

		bool hasData(Entity entity)
		{
			return entity_to_index_map.find(entity) != entity_to_index_map.end();
		}

		/// <summary>
		/// Remove an entity from all component arrays
		/// </summary>
		/// <param name="entity"> | The entity to remove from the component array </param>
		void removeData(Entity entity)
		{
			assert(entity_to_index_map.find(entity) != entity_to_index_map.end() &&
				"Removing non-existent component.");

			// Copy element at end into deleted element's place to maintain density
			size_t index_of_removed_entity = entity_to_index_map[entity];
			size_t index_of_last_element = size - 1;
			component_array[index_of_removed_entity] = component_array[index_of_last_element];

			// Update map to point to moved spot
			Entity entityOfLastElement = index_to_entity_map[index_of_last_element];
			entity_to_index_map[entityOfLastElement] = index_of_removed_entity;
			index_to_entity_map[index_of_removed_entity] = entityOfLastElement;

			entity_to_index_map.erase(entity);
			index_to_entity_map.erase(index_of_last_element);

			--size;
		}

		/// <summary>
		/// Return the component that an entity has 
		/// </summary>
		/// <param name="entity"> | The entity whos component to return </param>
		/// <returns>The component attached to the entity</returns>
		T& getData(Entity entity)
		{
			assert(entity_to_index_map.find(entity) != entity_to_index_map.end() &&
				"Retrieving non-existent component.");

			// Return a reference to the entity's component
			return component_array[entity_to_index_map[entity]];
		}

		/// <summary>
		/// Removes the componet entry from a destroyed entity
		/// </summary>
		/// <param name="entity"> | The entity whos component to destroy</param>
		void entityDestroyed(Entity entity) override
		{
			if (entity_to_index_map.find(entity) != entity_to_index_map.end())
			{
				// Remove the entity's component if it existed
				removeData(entity);
			}
		}

	private:
		/// <summary>
		/// The packed array of components has to match the maximum number of entities, 
		/// so that each entity has a unique spot
		/// </summary>
		std::array<T, MAX_ENTITIES> component_array;

		/// <summary>
		/// Map from an entity ID to an array index
		/// </summary>
		std::unordered_map<Entity, size_t> entity_to_index_map;

		/// <summary>
		/// Map from an array index to an entity ID
		/// </summary>
		std::unordered_map<size_t, Entity> index_to_entity_map;

		/// <summary>
		/// Total size of valid entries in the array
		/// </summary>
		size_t size;
	};
} // namespace Engine
