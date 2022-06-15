#pragma once

#include "StepTimer.h"

#include "System.h"
#include "Coordinator.h"

#include "Settlement.h"
#include "UnitSystem.h"
#include "Energy.h"

namespace Engine
{
	/// <summary>
	/// Spawns units from settlements that have been sprogged
	/// </summary>
	class SettlementSpawnerSystem : public Engine::System
	{
	public:
		SettlementSpawnerSystem();
		~SettlementSpawnerSystem() = default;
		/// <summary>
		/// Init the system, systems can't have constructors so required after creation
		/// </summary>
		/// <param name="_coordinator"> | Pointer to the scene's coordinator </param>
		/// <param name="_unit_system"> | Pointer to the scene's unit system </param>
		/// <param name="_on_collision_enter"> | Pointer to a fuction to run on the new unit's collision </param>
		/// <param name="_on_collision_exit"> | Pointer to a fuction to run on the new unit exiting collision </param>
		void init(Coordinator* _coordinator, UnitSystem* _unit_system, const std::function<void(const Engine::Entity&, const Engine::Entity&)>& _on_collision_enter, const std::function<void(const Engine::Entity&, const Engine::Entity&)>& _on_collision_exit); 
		/// <summary>
		/// Updates the system, checking if the units can settle
		/// </summary>
		/// <param name="timer">Driver's->Scene's StepTimer</param>
		void update(const DX::StepTimer& timer);
	private:
		/// <summary>
		/// Pointer to the scene's coordinator
		/// </summary>
		Coordinator* coordinator = nullptr;
		/// <summary>
		/// Pointer to the scene's unit system
		/// </summary>
		UnitSystem* unit_system = nullptr;
		/// <summary>
		/// Empty function to point to if uninitialized
		/// </summary>
		void empty_template(const Engine::Entity&, const Engine::Entity&) {};
		std::function<void(const Engine::Entity&, const Engine::Entity&)> on_collision_func = [this](Entity, Entity) {empty_template(NULL_ENTITY, NULL_ENTITY); };
		std::function<void(const Engine::Entity&, const Engine::Entity&)> off_collision_func = [this](Entity, Entity) { empty_template(NULL_ENTITY, NULL_ENTITY); };
	};
}
