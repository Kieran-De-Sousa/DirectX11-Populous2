// Created by Ben 12/03/2022

#pragma once

// DirectX
#include "StepTimer.h"

// Project
#include "TileMap.h"
#include "PathFinder.h"
#include "System.h"
#include "Coordinator.h"

#include "RenderSystem.h"
#include "SharedData.h"

namespace Engine
{
	/// <summary>
	/// Allows the units to place houses 
	/// </summary>
	class SettleSystem : public Engine::System
	{
	public:
		SettleSystem();
		~SettleSystem() = default;

		/// <summary>
		/// Init the system, systems can't have constructors so required after creation
		/// </summary>
		/// <param name="_coordinator"> | Pointer to the scene's coordinator </param>
		/// <param name="_tile_map"> | Pointer to the scene's tile map</param>
		/// <param name="_path_finder"> | Pointer to the scene's pathfinder</param>
		/// <param name="_renderer"> | Pointer to the scenes's render system</param>
		void init(Coordinator* _coordinator, TileMap* _tile_map, PathFinder* _path_finder, RenderSystem* _renderer);
		/// <summary>
		/// Updates the system, checking if the units can settle
		/// </summary>
		/// <param name="timer">Driver's->Scene's StepTimer</param>
		void update(const DX::StepTimer& timer);

	private:
		/// <summary>
		/// Checks if the tile is Grass
		/// </summary>
		bool simpleSettleCheck(int x, int y);
		/// <summary>
		/// Checks if the tile is Grass and no surrounding tiles have already been settled
		/// </summary>
		bool complexSettleCheck(int x, int y);
		/// <summary>
		/// Checks if the x and y are valid numbers for the Tile Map
		/// </summary>
		bool onTheGrid(int x, int y);
		/// <summary>
		/// Pointer to the scene's coordinator
		/// </summary>
		Coordinator* coordinator;
		/// <summary>
		/// Pointer to the scene's tilemap
		/// </summary>
		TileMap* tile_map;                // Again, this definitely isn't ideal but I still can't think of a better solution 
		/// <summary>
		/// Pointer to the scene's pathfinder
		/// </summary>
		PathFinder* path_finder;          // to pass the data we need around yet
		/// <summary>
		/// Pointer to the scene's render system
		/// </summary>
		RenderSystem* render_system;
		/// <summary>
		/// List of all of the tile types settlers can settle on
		/// </summary>
		TypeMaps MAPPER;

		void onCollisionEnter(const Engine::Entity& t, const Engine::Entity& v);
		void onCollisionExit(const Engine::Entity& t, const Engine::Entity& v);
	};
} // namespace Engine
