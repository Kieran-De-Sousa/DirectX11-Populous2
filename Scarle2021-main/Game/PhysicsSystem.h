// Created by Ben on 22/02/2022
// I don't actually know how much actual physics will be in the physics system... but it sounds cool

#pragma once

#include "StepTimer.h"

#include "System.h"
#include "Coordinator.h"

namespace Engine
{
	/// <summary>
	/// A basic physics system 
	/// </summary>
	class PhysicsSystem : public Engine::System
	{
	public:
		PhysicsSystem();
		~PhysicsSystem() = default;

		/// <summary>
		/// Init the system, systems can't have constructors so required after creation
		/// </summary>
		/// <param name="_coordinator"> | The scene's coordinator </param>
		void init(Coordinator* _coordinator);
		/// <summary>
		/// Updates the system, moving entity's transforms based off their rigidbody
		/// </summary>
		/// <param name="timer"> | Drivers->Scene's steptimer </param>
		void update(const DX::StepTimer& timer);

	private:
		Coordinator* coordinator;
	};
}
