// Created by Ben on 28 / 02 / 2022

#pragma once

#include "StepTimer.h"

#include "System.h"
#include "Coordinator.h"

namespace Engine
{
	/// <summary>
	/// A basic camera system which updates the cameras projection matrixs based upon their position
	/// </summary>
	class CameraSystem : public Engine::System
	{
	public:
		CameraSystem();
		~CameraSystem() = default;

		/// <summary>
		/// Init the system, systems can't have constructors so required after creation
		/// </summary>
		/// <param name="_coordinator"> | The scene's coordinator</param>
		void init(Coordinator * _coordinator);
		/// <summary>
		/// Updates the system, updating the projection matrix of the camera
		/// </summary>
		/// <param name="timer">Driver->Scene's step timer</param>
		void update(const DX::StepTimer & timer);

	private:
		/// <summary>
		/// Pointer to the scene's coordinator
		/// </summary>
		Coordinator* coordinator;
	};
}
