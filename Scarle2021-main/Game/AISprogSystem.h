#pragma once

#include "StepTimer.h"

#include "System.h"
#include "Coordinator.h"

#include "SharedData.h"

#include "Energy.h"
#include "Settlement.h"
#include "SettlementAIWatch.h"

namespace Engine
{
	/// <summary>
	/// Using AI data, it creates a chance to alert the Settlement Spawner System through the Settlement component to spawn a new unit
	/// </summary>
	class AISprogSystem : public Engine::System
	{
	public:

		AISprogSystem();
		~AISprogSystem() = default;

		/// <summary>
		/// Init the system, systems can't have constructors so required after creation
		/// </summary>
		/// <param name="_coordinator"> | Pointer to the scene's coordinator </param>
		void init(Coordinator* _coordinator);
		/// <summary>
		/// Updates the system, rolls for any settlement with an expired timer, resets timer on fail or sets a boolean value in their Settlement component on success
		/// </summary>
		/// <param name="timer">Driver's->Scene's StepTimer</param>
		void update(const DX::StepTimer& timer);

		/// <summary>
		/// Disables update until enableAI() is called and removes all watches
		/// </summary>
		void disableAI();
		/// <summary>
		/// Re-enables update
		/// </summary>
		void enableAI();

		//void changeBehaviour(SprogAIData new_behaviour, const bool& clear_watches); Temporarily removed

		/// <summary>
		/// Adds an AI behaviour based on the data set provided
		/// </summary>
		/// /// <param name="timer">SprogAIData from SharedData.h, defines a linearly increasing chance based on energy, and defines a minimum threshold</param>
		void addAIData(SprogAIData new_ai_data);

	private:
		//std::vector<std::pair<Entity, float>> on_watch; Moved to SettlementAIWatch component
		bool enabled = true;
		Coordinator* coordinator = nullptr;
		std::vector<SprogAIData> ai_data;
	};
}
