#pragma once

#include "StepTimer.h"

#include "System.h"
#include "Coordinator.h"

#include "TeamController.h"
#include "ManaPool.h"
#include "Settlement.h"

namespace Engine
{
	/// <summary>
	/// A system increasing the mana pool of the teams based on settlements
	/// </summary>
	class WorshipSystem : public Engine::System
	{
	public:
		WorshipSystem();
		~WorshipSystem() = default;
		/// <summary>
		/// Init the system, systems can't have constructors so required after creation
		/// </summary>
		/// <param name="_coordinator"> | Pointer to the scene's coordinator </param>
		/// <param name="_mana_per_house"> | Amount of mana generated per house per second</param>
		void init(Coordinator* _coordinator, float _mana_per_house);
		/// <summary>
		/// Updates the system, adds mana to the respective controllers' mana pool 
		/// </summary>
		/// <param name="timer">Driver's->Scene's StepTimer</param>
		void update(const DX::StepTimer& timer);
		/// <summary>
		/// Registers a team controller with the system to feed mana into.
		/// Team controller entities need to have ManaPool components and TeamController tags.
		/// </summary>
		/// <param name="team_controller"> | The team's controller entity</param>
		void registerController(const Entity& team_controller); //registering solution, as opposed to the pointer initialisation in AbilityAI
	private:
		/// <summary>
		/// Pointer to the scene's coordinator
		/// </summary>
		Coordinator* coordinator = nullptr;
		/// <summary>
		/// Storing the initialized mana per house per second value
		/// </summary>
		float mana_per_house = 0.025F;
		/// <summary>
		/// List of team controllers to keep track of
		/// </summary>
		std::vector<Entity> controllers;
	};
}
