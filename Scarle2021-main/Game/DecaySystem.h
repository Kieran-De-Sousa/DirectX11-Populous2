#pragma once

#include "StepTimer.h"

#include "System.h"
#include "Coordinator.h"

#include "EnergyDecay.h"
//#include "Energy.h"
#include "Unit.h"

namespace Engine
{
	/// <summary>
	/// Decreases energy of units and calls for their destruction when they run out / starve to death
	/// </summary>
	class DecaySystem : public Engine::System
	{
	public:
		DecaySystem();
		~DecaySystem() = default;
		/// <summary>
		/// Init the system, systems can't have constructors so required after creation
		/// </summary>
		/// <param name="_coordinator"> | Pointer to the scene's coordinator </param>
		void init(Coordinator* _coordinator);
		/// <summary>
		/// Updates the system, decreases energy of it's entity list, if any of the entities drop to 0 or below, call for their destruction
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
