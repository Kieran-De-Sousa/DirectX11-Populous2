#pragma once

#include "StepTimer.h"

#include "System.h"
#include "Coordinator.h"
#include "TileMap.h"

#include "SharedData.h"
#include "Unit.h"
#include "Transform.h"
#include "PathFinderAgent.h"

namespace Engine
{
	/// <summary>
	/// Destroys units that are off the map (tornado power, pathfinding error, etc)
	/// </summary>
	class BoundarySystem : public Engine::System
	{
	public:
		BoundarySystem();
		~BoundarySystem() = default;

		/// <summary>
		/// Init the system, systems can't have constructors so required after creation
		/// </summary>
		/// <param name="_coordinator"> | Pointer to the scene's coordinator </param>
		void init(Coordinator* _coordinator, TileMap* _tile_map);
		/// <summary>
		/// Updates the system, calls the destruction of entities that are outside the tilemap according to the tilemap
		/// </summary>
		/// <param name="timer">Driver's->Scene's StepTimer</param>
		void update(const DX::StepTimer& timer);
	private:
		Coordinator* coordinator = nullptr;
		TileMap* tile_map = nullptr;
		/// <summary>
		/// Keeps track on how many times it has destroyed entities for debugging, could add a vector to push back locations?
		/// </summary>
		size_t counter = 0;
	};
}
