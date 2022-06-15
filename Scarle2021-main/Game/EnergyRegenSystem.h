#pragma once

#include "StepTimer.h"

#include "System.h"
#include "Coordinator.h"

#include "Energy.h"
#include "Settlement.h"

namespace Engine
{
	/// <summary>
	/// Increases the energy of settlements
	/// </summary>
	class EnergyRegenSystem : public Engine::System
	{
	public:
		EnergyRegenSystem();
		~EnergyRegenSystem() = default;
		/// <summary>
		/// Init the system, systems can't have constructors so required after creation
		/// </summary>
		/// <param name="_coordinator"> | Pointer to the scene's coordinator </param>
		void init(Coordinator* _coordinator);
		/// <summary>
		/// Updates the system, increases the settlement energy based on the timer and the settlement's energy regen
		/// </summary>
		/// <param name="timer">Driver's->Scene's StepTimer</param>
		void update(const DX::StepTimer& timer);
	private:
		/// <summary>
		/// Pointer to the scene's coordinator
		/// </summary>
		Coordinator* coordinator = nullptr;
	};
}
