// Created by Ben on 21/02/2022
// Resources: https://youtu.be/rX0ItVEVjHc | https://austinmorlan.com/posts/entity_component_system/ 

#pragma once

// System
#include <memory>
#include <iostream>

// Project
#include "ComponentManager.h"
#include "EntityManager.h"
#include "EventManager.h"
#include "SystemManager.h"
#include "Types.h"

namespace Engine
{
	/// <summary>
	/// The Coordinator has a pointer to each ComponentManager and can in turn do some managing between them.
	/// As it keeps everything nicely in one place, its also the thing we return(ed) from the ECS singleton
	/// </summary>
	class Coordinator
	{
	public:
		Coordinator() = default;
		~Coordinator() = default;

		/// <summary>
		/// Create pointers to each manager
		/// </summary>
		void init()
		{
			component_manager = std::make_unique<ComponentManager>();
			entity_manager = std::make_unique<EntityManager>();
			system_manager = std::make_unique<SystemManager>();

			delete_queue.reserve(INITIAL_DELETE_QUEUE_SIZE);
		}

		/// <summary>
		/// Iterates through the delete_queue and destroys any entities in it at the end of the frame
		/// </summary>
		void update()
		{
			if (delete_entities_at_end_of_frame)
			{
				auto it = delete_queue.begin();

				while (it != delete_queue.end())
				{
					destroyEntity(*it);
					it = delete_queue.erase(it);

					//std::cout << "Destruction called." << std::endl;
					// Need to remove?
				}

				delete_entities_at_end_of_frame = false;
			}
		}

		// --- Entity Methods --- 

		/// <summary>
		/// Creates an entity
		/// </summary>
		/// <returns>The created entity</returns>
		Entity createEntity()
		{
			return entity_manager->createEntity();
		}

		/// <summary>
		/// Creates a child of an entity
		/// </summary>
		/// <param name="parent">The entity's parent</param>
		/// <returns>The new child entity</returns>
		Entity createChild(Entity parent)
		{
			return entity_manager->createChild(parent);
		}

		/// <summary>
		/// Creates a parent for an entity
		/// </summary>
		/// <param name="parent"> | The new parent </param>
		/// <param name="child"> | The forced child </param>
		void addParent(Entity parent, Entity child)
		{
			entity_manager->addParent(parent, child);
		}

		/// <summary>
		/// Finds all children on a parent entity
		/// </summary>
		/// <param name="parent"> | The entity whos (possible) children to find </param>
		/// <returns>A vector of all the entities children</returns>
		std::vector<Entity> findChildren(Entity parent)
		{
			return entity_manager->findChildren(parent);
		}

		/// <summary>
		/// Destroys an entity and all of its children
		/// </summary>
		/// <param name="entity"> | The entity to destroy </param>
		void destroyEntity(Entity entity)
		{
			// Destroy children before destroying the entity
			for (auto& child : findChildren(entity))
			{
				destroyEntity(child);
			}

			entity_manager->destroyEntity(entity);

			component_manager->entityDestroyed(entity);

			system_manager->entityDestroyed(entity);
		}

		/// <summary>
		/// Destroy the entity a the end of the frame
		/// Important if other sytems could be iterating over that entity when the entity is destroyed
		/// </summary>
		/// <param name="entity"> | The entity to destory at the end of the frame</param>
		void destroyEntityAtEndOfFrame(Entity entity)
		{
			// ALREADY HANDLES CHILDREN, DONT TRY TO FIX IT AGAIN; To: Me, from Dear me
			delete_queue.push_back(entity);
			delete_entities_at_end_of_frame = true;
		}

		// --- Component Methods ---

		/// <summary>
		/// Registers a component with the ECS. 
		/// Must be called for every component 
		/// </summary>
		/// <typeparam name="T"> | The type of component </typeparam>
		template<typename T>
		void registerComponent()
		{
			component_manager->registerComponent<T>();
		}

		/// <summary>
		/// Adds a component to an entity 
		/// </summary>
		/// <typeparam name="T"> | The type of component </typeparam>
		/// <param name="entity"> | The entity to add the component to </param>
		/// <param name="component"> | The component </param>
		/// <returns> The added component </returns>
		template<typename T>
		T& addComponent(Entity entity, T component)
		{
			component_manager->addComponent<T>(entity, component);

			auto signature = entity_manager->getSignature(entity);
			signature.set(component_manager->getComponentType<T>(), true);
			entity_manager->setSignature(entity, signature);

			system_manager->entitySignatureChanged(entity, signature);
			return getComponent<T>(entity);
		}

		/// <summary>
		/// Removes a component from an entity 
		/// </summary>
		/// <typeparam name="T"> | The type of component </typeparam>
		/// <param name="entity"> | The entity to remove the component from</param>
		template<typename T>
		bool hasComponent(Entity entity)
		{
			return component_manager->hasComponent<T>(entity);
		}

		template<typename T>
		void removeComponent(Entity entity)
		{
			component_manager->removeComponent<T>(entity);

			auto signature = entity_manager->getSignature(entity);
			signature.set(component_manager->getComponentType<T>(), false);
			entity_manager->setSignature(entity, signature);

			system_manager->entitySignatureChanged(entity, signature);
		}

		/// <summary>
		/// Returns a component from an entity
		/// </summary>
		/// <typeparam name="T"> | The type of componentn</typeparam>
		/// <param name="entity"> | The entity to get the component from </param>
		/// <returns>The component</returns>
		template<typename T>
		T& getComponent(Entity entity)
		{
			return component_manager->getComponent<T>(entity);
		}

		/// <summary>
		/// Returns the compontent type from component manager
		/// </summary>
		/// <typeparam name="T">| The type of component</typeparam>
		/// <returns>The ComponentType</returns>
		template<typename T>
		ComponentType getComponentType()
		{
			return component_manager->getComponentType<T>();
		}

		// --- System Methods ---

		/// <summary>
		/// Registers a system with the ECS
		/// Must be done for all systems
		/// </summary>
		/// <typeparam name="T"> | The type of system </typeparam>
		/// <returns> A shared pointer to the newly registered system </returns>
		template<typename T>
		std::shared_ptr<T> registerSystem()
		{
			return system_manager->registerSystem<T>();
		}

		/// <summary>
		/// Set s systems signature
		/// Defines which components an entity needs to be run through the system
		/// </summary>
		/// <typeparam name="T"> | The system</typeparam>
		/// <param name="signature">| The signature to assign to the system</param>
		template<typename T>
		void setSystemSignature(Signature signature)
		{
			system_manager->setSignature<T>(signature);
		}

	private:
		/// <summary>
		/// Pointer to the component manager 
		/// </summary>
		std::unique_ptr<ComponentManager> component_manager;
		/// <summary>
		/// Pointer to the entity manager
		/// </summary>
		std::unique_ptr<EntityManager> entity_manager;
		/// <summary>
		/// Pointer to the system manager
		/// </summary>
		std::unique_ptr<SystemManager> system_manager;

		/// <summary>
		/// Entities to delete at the end of the frame
		/// </summary>
		const unsigned int INITIAL_DELETE_QUEUE_SIZE = 100;
		std::vector<Entity> delete_queue{};
		/// <summary>
		/// If true, the delete_queue will be iterated through at the end of the frame
		/// </summary>
		bool delete_entities_at_end_of_frame = false;
	};
} // namespace Engine
