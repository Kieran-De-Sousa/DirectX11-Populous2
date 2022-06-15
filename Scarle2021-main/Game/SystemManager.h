// Created by Ben on 21/02/2022
// Resources: https://youtu.be/rX0ItVEVjHc | https://austinmorlan.com/posts/entity_component_system/ 

#pragma once

// System
#include <cassert>
#include <memory>
#include <unordered_map>

// Project
#include "System.h"
#include "Types.h"

namespace Engine
{
	/// <summary>
	/// Similar to ComponentManager, but for Systems. You have to use registerSystem for every system added
	/// </summary>
	class SystemManager
	{
	public:
		SystemManager() = default;
		~SystemManager() = default;

		/// <summary>
		/// Registers a system with the ECS. Required for every system. 
		/// </summary>
		/// <typeparam name="T"> | The type of system</typeparam>
		/// <returns>A shared pointer to the registered system</returns>
		template<typename T>
		std::shared_ptr<T> registerSystem()
		{
			const char* type_name = typeid(T).name();

			assert(systems.find(type_name) == systems.end() && "Registering system more than once.");

			// Create a pointer to the system and return it so it can be used externally
			auto system = std::make_shared<T>();
			systems.insert({ type_name, system });
			return system;
		}

		/// <summary>
		/// Registers a signature for a system
		/// </summary>
		/// <typeparam name="T"> | The system </typeparam>
		/// <param name="signature"> | The signature </param>
		template<typename T>
		void setSignature(Signature signature)
		{
			const char* type_name = typeid(T).name();

			assert(systems.find(type_name) != systems.end() && "System used before registered.");

			// Set the signature for this system
			signatures.insert({ type_name, signature });
		}

		/// <summary>
		/// Removes an entity from all system lists
		/// </summary>
		/// <param name="entity"> | The entity to remove</param>
		void entityDestroyed(Entity entity)
		{
			// Erase a destroyed entity from all system lists, entities is a set so no check needed
			for (auto const& pair : systems)
			{
				auto const& system = pair.second;

				system->entities.erase(entity);
			}
		}

		/// <summary>
		/// Tell every system that an entitis signature has changed 
		/// </summary>
		/// <param name="entity"> | The entity whos signature has chagned </param>
		/// <param name="entitySignature"> | The new entity signature </param>
		void entitySignatureChanged(Entity entity, Signature entitySignature)
		{
			// Notify each system that an entity's signature changed
			for (auto const& pair : systems)
			{
				auto const& type = pair.first;
				auto const& system = pair.second;
				auto const& system_signature = signatures[type];

				// Entity signature matches system signature - insert into set
				if ((entitySignature & system_signature) == system_signature)
				{
					system->entities.insert(entity);
				}
				// Entity signature does not match system signature - erase from set
				else
				{
					system->entities.erase(entity);
				}
			}
		}

	private:
		/// <summary>
		/// Map from system type string pointer to a signature
		/// </summary>
		std::unordered_map<const char*, Signature> signatures{};

		/// <summary>
		/// Map from system type string pointer to a system pointer
		/// </summary>
		std::unordered_map<const char*, std::shared_ptr<System>> systems{};
	};
} // namespace Engine

// Const char because slightly more memory efficient https://www.geeksforgeeks.org/char-vs-stdstring-vs-char-c/
