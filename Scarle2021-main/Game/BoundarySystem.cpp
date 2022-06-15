#include "pch.h"

#include "BoundarySystem.h"

#include <iostream>

Engine::BoundarySystem::BoundarySystem() : coordinator(nullptr)
{
	// Can't initialise coordinator in here because of header only circular dependency issues
}

void Engine::BoundarySystem::init(Coordinator* _coordinator, TileMap* _tile_map)
{
	coordinator = _coordinator;
	tile_map = _tile_map;
}

void Engine::BoundarySystem::update(const DX::StepTimer& timer)
{
	for (const auto& entity : entities)
	{
		Transform& transform = coordinator->getComponent<Transform>(entity);
		Engine::Vector2Int current_position = tile_map->worldToTile(transform.position);
		if (current_position.x >= tile_map->getWidth() || current_position.y >= tile_map->getHeight())
		{
			coordinator->destroyEntityAtEndOfFrame(entity);
			std::cout << "Entity " << entity << ": Fell off the map" << std::endl;
			counter++;
			//coordinator->removeComponent<PathFinderAgent>(entity);
		}
	}
}

