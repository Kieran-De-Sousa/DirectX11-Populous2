#pragma once

#include "StepTimer.h"

#include "System.h"
#include "Coordinator.h"

#include "RenderSystem.h" //coupled systems bad

#include "Settlement.h"
#include "SpriteRenderer.h"
#include "Transform.h"
#include "EnergyFlagComponent.h"
#include "Energy.h"


namespace Engine
{
	/// <summary>
	/// Creates flags entities for the settlements that don't have 
	/// </summary>
	class FlagCreationSystem : public Engine::System
	{
	public:
		FlagCreationSystem();
		~FlagCreationSystem() = default;
		/// <summary>
		/// Init the system, systems can't have constructors so required after creation
		/// </summary>
		/// <param name="_coordinator"> | Pointer to the scene's coordinator </param>
		/// <param name="_renderer"> | Pointer to the scenes's render system</param>
		void init(Coordinator* _coordinator, RenderSystem* _renderer);
		/// <summary>
		/// Updates the system, checks if the settlement has a flag assigned yet, if not, then create one
		/// </summary>
		/// <param name="timer">Driver's->Scene's StepTimer</param>
		void update(const DX::StepTimer& timer);
	private:
		/// <summary>
		/// Pointer to the scene's coordinator
		/// </summary>
		Coordinator* coordinator = nullptr;
		/// <summary>
		/// Pointer to the scene's render system
		/// </summary>
		RenderSystem* renderer = nullptr;
	};
}
