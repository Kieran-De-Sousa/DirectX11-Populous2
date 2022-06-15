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
	/// Adds watch(SettlementAIWatch) components to settlements
	/// </summary>
	class AISettlementWatchSystem : public Engine::System
	{
	public:
		/// <summary>
		/// Was split from AI Sprog system, so it still has some unnecessary data shared and functionality
		/// </summary>
		AISettlementWatchSystem();
		~AISettlementWatchSystem() = default;

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
		/// <summary>
		/// Adds an AI behaviour based on the data set provided
		/// </summary>
		/// /// <param name="timer">SprogAIData from SharedData.h, defines a linearly increasing chance based on energy, and defines a minimum threshold</param>
		void addTeamAI(SprogAIData new_ai_data);
	private:
		bool enabled = true;
		Coordinator* coordinator = nullptr;
		
		std::vector<SprogAIData> ai_data;
	};
}
