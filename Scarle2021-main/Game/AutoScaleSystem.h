#pragma once

#include <SimpleMath.h>

#include "DrawData2D.h"
#include "CommonStates.h"
#include "StepTimer.h"

#include "System.h"
#include "Coordinator.h"

namespace Engine
{
	/// <summary>
	/// Rescales all entities containing a transform to scale with the screens dimensions
	/// </summary>
	class AutoScaleSystem : public Engine::System
	{
	public:
		AutoScaleSystem();
		~AutoScaleSystem() = default;

		/// <summary>
		/// Init the system, systems can't have constructors so required after creation
		/// </summary>
		/// <param name="_coordinator">| The scene's coordinator</param>
		void init(Coordinator* _coordinator);
		/// <summary>
		/// Update the system, rescaling entity transforms if window dimensions have changed since the last tick
		/// </summary>
		/// <param name="timer">| Delta time</param>
		void update(DX::StepTimer const& timer);

	private:
		// ECS
		Coordinator* coordinator;
		DrawData2D* draw_data;
		CommonStates* common_states;
		ID3D11Device1* d3d_device;

		// Viewport
		const DirectX::SimpleMath::Vector2 ASPECT_RATIO = { 16, 9 };
		///DirectX::SimpleMath::Vector2 previousDimensions = { 0, 0 };
		DirectX::SimpleMath::Vector2 windowDimensions = { 0, 0 };
	};
}