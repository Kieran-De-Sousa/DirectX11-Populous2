#pragma once

#include <deque>

#include <SimpleMath.h>

namespace Engine
{
	/// <summary>
	/// Holds pathfinding data for a unit. Base component that all units have
	/// </summary>
	struct PathFinderAgent
	{
		PathFinderAgent() = default;
		~PathFinderAgent() = default;

		std::deque<DirectX::SimpleMath::Vector3> path;
		float speed = 20;
		float sensitivity = 10; // How close the AI has to get to its target destination before it calculates a new path
	};
}
