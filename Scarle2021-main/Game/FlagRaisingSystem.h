#pragma once

#include "StepTimer.h"

#include "System.h"
#include "Coordinator.h"

#include "Transform.h"
#include "EnergyFlagComponent.h"
#include "Energy.h"

namespace Engine
{
	/// <summary>
	/// Raises the flag entities of settlements according to their energy
	/// </summary>
	class FlagRaisingSystem : public Engine::System
	{
	public:
		FlagRaisingSystem();
		~FlagRaisingSystem() = default;
		/// <summary>
		/// Init the system, systems can't have constructors so required after creation
		/// </summary>
		/// <param name="_coordinator"> | Pointer to the scene's coordinator </param>
		void init(Coordinator* _coordinator);
		/// <summary>
		/// Updates the system, setting the flag entity's position in relation to the current energy / max energy ratio
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
