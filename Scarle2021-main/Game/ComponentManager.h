// Created by Ben on 21/02/2022
// Resources: https://youtu.be/rX0ItVEVjHc | https://austinmorlan.com/posts/entity_component_system/ 

#pragma once

// System
#include <memory>
#include <unordered_map>

// Program
#include "ComponentArray.h"
#include "Types.h"

namespace Engine
{
	/// <summary>
	/// Manages all of the ComponentArrays. There is one ComponentArray per instantiation of ComponentType
	/// Have to call registerComponent everytime you add a component to an entity and want to use it
	/// </summary>
	class ComponentManager
	{
	public:
		ComponentManager() = default;
		~ComponentManager() = default;

		/// <summary>
		/// Registers a component, called from Coordinator
		/// </summary>
		/// <typeparam name="T"> | The type of component </typeparam>
		template<typename T>
		void registerComponent()
		{
			const char* type_name = typeid(T).name();

			assert(component_types.find(type_name) == component_types.end() && "Registering component type more than once.");

			// Add this component type to the component type map
			component_types.insert({ type_name, next_component_type });

			// Create a ComponentArray pointer and add it to the component arrays map
			component_arrays.insert({ type_name, std::make_shared<ComponentArray<T>>() });

			// Increment the value so that the next component registered will be different
			++next_component_type;
		}

		/// <summary>
		/// Gets the type of a component, called from Coordinator
		/// </summary>
		/// <typeparam name="T"> | The type of component </typeparam>
		/// <returns> The ComponentType </returns>
		template<typename T>
		Engine::ComponentType getComponentType()
		{
			const char* type_name = typeid(T).name();

			assert(component_types.find(type_name) != component_types.end() && "Component not registered before use.");

			// Return this component's type - used for creating signatures
			return component_types[type_name];
		}

		/// <summary>
		/// Adds a component to an entity, called from Coordinator
		/// </summary>
		/// <typeparam name="T"> | The component type </typeparam>
		/// <param name="entity"> | The entity to add the component to </param>
		/// <param name="component"> | The component </param>
		template<typename T>
		void addComponent(Engine::Entity entity, T component)
		{
			// Add a component to the array for an entity
			getComponentArray<T>()->insertData(entity, component);
		}

		/// <summary>
		/// Removes a component from an entity, called from Coordinator
		/// </summary>
		/// <typeparam name="T"> | The type of component </typeparam>
		/// <param name="entity"> | The entity to remove the component from </param>
		template<typename T>
		void removeComponent(Engine::Entity entity)
		{
			// Remove a component from the array for an entity
			getComponentArray<T>()->removeData(entity);
		}

		/// <summary>
		/// Get a component attached to an entity, called from Coordinator
		/// </summary>
		/// <typeparam name="T"> | The type of component </typeparam>
		/// <param name="entity"> | The entity to get the component from </param>
		/// <returns> The component </returns>
		template<typename T>
		bool hasComponent(Engine::Entity entity)
		{
			return getComponentArray<T>()->hasData(entity);
		}

		template<typename T>
		T& getComponent(Engine::Entity entity)
		{
			// Get a reference to a component from the array for an entity
			return getComponentArray<T>()->getData(entity);
		}

		/// <summary>
		/// Notify each component array that an entity has been destroyed
		/// If it has a component for that entity, it will remove it
		/// </summary>
		/// <param name="entity"> | The entity to remove the component from </param>
		void entityDestroyed(Engine::Entity entity)
		{
			for (auto const& pair : component_arrays)
			{
				auto const& component = pair.second;

				component->entityDestroyed(entity);
			}
		}

	private:
		/// <summary>
		/// Map from type string pointer to a component type
		/// </summary>
		std::unordered_map<const char*, Engine::ComponentType> component_types{};

		/// <summary>
		/// Map from type string pointer to a component array
		/// </summary>
		std::unordered_map<const char*, std::shared_ptr<IComponentArray>> component_arrays{};

		/// <summary>
		/// The component type to be assigned to the next registered component - starting at 0
		/// </summary>
		Engine::ComponentType next_component_type{};

		/// <summary>
		/// Convenience function to get the statically casted pointer to the ComponentArray of type T
		/// </summary>
		template<typename T>
		std::shared_ptr<ComponentArray<T>> getComponentArray()
		{
			const char* type_name = typeid(T).name();

			assert(component_types.find(type_name) != component_types.end() && "Component not registered before use.");

			return std::static_pointer_cast<ComponentArray<T>>(component_arrays[type_name]);
		}
	};
} // namespace Engine

// Const char because slightly more memory efficient https://www.geeksforgeeks.org/char-vs-stdstring-vs-char-c/
