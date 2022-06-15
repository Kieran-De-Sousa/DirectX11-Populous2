// Created by Ben on 21/02/2022
// Resources: https://youtu.be/rX0ItVEVjHc | https://austinmorlan.com/posts/entity_component_system/ 

#pragma once

// System
#include <array>
#include <cassert>
#include <queue>

// Project
#include "Types.h"

namespace Engine
{
	/// <summary>
	/// The EntityManagern in charge of distributing entity IDs and keeping record of which IDs are in use and which are not
	/// </summary>
	class EntityManager
	{
	public:
		/// <summary>
		/// Intialises the available_entities queue with MAX_ENTITIES
		/// </summary>
		EntityManager()
		{
			// Initialize the queue with all possible entity IDs
			for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
			{
				available_entities.push(entity);
			}
			// Initialize the relationship vector with a reasonable size so resizing is unlikely, but doesn't take up a huge chunk of memory
			parent_to_child.reserve(MAX_ENTITIES);
		}
		~EntityManager() = default;

		/// <summary>
		/// Assigns an entity ID to a entity. Doesn't really 'create' anything...
		/// </summary>
		/// <returns>The ID of the new entity</returns>
		Entity createEntity()
		{
			assert(living_entity_count < MAX_ENTITIES && "Too many entities in existence.");

			// Take an ID from the front of the queue
			Entity id = available_entities.front();
			available_entities.pop();
			++living_entity_count;

			return id;
		}

		/// <summary>
		/// Creates a child of an entity
		/// </summary>
		/// <param name="parent"> | The parent </param>
		/// <returns> The child entity's ID </returns>
		Entity createChild(Entity parent)
		{
			Entity new_child = createEntity();
			addParent(parent, new_child);
			return new_child;
		}

		/// <summary>
		/// Resets the entity's ID so its free to be assigned to something else. Doesn't really 'destory' anything...
		/// </summary>
		/// <param name="entity">The entity to destroy</param>
		void destroyEntity(Entity entity)
		{
			assert(entity < MAX_ENTITIES && "Entity out of range.");

			// Remove all relations
			removeAllRelations(entity);

			// Invalidate the destroyed entity's signature
			signatures[entity].reset();

			// Put the destroyed ID at the back of the queue
			available_entities.push(entity);
			--living_entity_count;
		}

		/// <summary>
		/// Sets an entities signature so systems can iterate over it correctly
		/// </summary>
		/// <param name="entity"> | The entity whos entity to set </param>
		/// <param name="signature"> | The signature </param>
		void setSignature(Entity entity, Signature signature)
		{
			assert(entity < MAX_ENTITIES && "Entity out of range.");

			// Put this entity's signature into the array
			signatures[entity] = signature;
		}

		/// <summary>
		/// Returns an entities signature
		/// </summary>
		/// <param name="entity"> | The entity whos signature to remove </param>
		/// <returns> The passed in entity's signature </returns>
		Signature getSignature(Entity entity)
		{
			assert(entity < MAX_ENTITIES && "Entity out of range!"); //we got an exclamation mark here
			                                                         //thanks dude i thought i would add some emotion to my code
			// Get this entity's signature from the array
			return signatures[entity];
		}

		/// <summary>
		/// Makes an entity a child by assigning it a parent
		/// </summary>
		/// <param name="parent"> | The new parent </param>
		/// <param name="child"> | The new child </param>
		void addParent(Entity parent, Entity child)
		{
			assert(parent != child && "Entity can't be it's own parent");
			parent_to_child.push_back(std::make_pair(parent, child));
		}

		/// <summary>
		/// Remove all parent and child relations from an entity
		/// </summary>
		/// <param name="target"> | The entity whos relations to remove </param>
		void removeAllRelations(Entity target)
		{
			// Create a queue for deletion to avoid invalidating the loop
			std::vector<size_t> deletion_queue;
			// Record which relations are deleted
			size_t index_helper = 0;
			for (auto& relationship : parent_to_child)
			{
				if (relationship.first == target || relationship.second == target)
				{
					deletion_queue.push_back(index_helper);
				}
				index_helper++;
			}

			// Remove relations from the vector
			for (auto& removal : deletion_queue)
			{
				parent_to_child.erase(parent_to_child.begin() + removal);
				// Once an index is removed, all later indexes need to be moved up, otherwise unrelated relations might get removed
				for (auto& index_fixing : deletion_queue)
				{
					if (index_fixing > removal)
					{
						index_fixing--;
					}
				}
			}
		}

		/// <summary>
		/// Return the first child of an entity
		/// </summary>
		/// <param name="possible_parent"> | The entity whos (possible) children to check </param>
		/// <returns>The [possible_parent]'s first entity</returns>
		Entity findFirstChild(Entity possible_parent)
		{
			// Try to find a child
			for (auto& relationship : parent_to_child)
			{
				if (relationship.first == possible_parent)
				{
					return relationship.second;
				}
			}
			// Return null or itself to signal no children found
			//return possible_parent;
			return NULL_ENTITY;
		}

		/// <summary>
		/// Returns all the children of an entity
		/// </summary>
		/// <param name="possible_parent"> | The entity whos (possible) children to return</param>
		/// <returns>A vector of the entities children</returns>
		std::vector<Entity> findChildren(Entity possible_parent)
		{
			std::vector<Entity> family; // Is this correct naming? In weird territory with naming these stuff
			for (auto& relationship : parent_to_child)
			{
				if (relationship.first == possible_parent)
				{
					family.push_back(relationship.second);
				}
			}
			return family;
		}

	private:
		/// <summary>
		/// Queue of unused entity IDs
		/// </summary>
		std::queue<Entity> available_entities{};

		/// <summary>
		/// Array of signatures where the index corresponds to the entity ID
		/// </summary>
		std::array<Signature, MAX_ENTITIES> signatures{};

		/// <summary>
		/// Total living entities - used to keep limits on how many exist
		/// </summary>
		Entity living_entity_count{};

		/// <summary>
		/// Relationship map of the entities
		/// </summary>
		std::vector<std::pair<Entity, Entity>> parent_to_child;
	};
} // namespace Engine
