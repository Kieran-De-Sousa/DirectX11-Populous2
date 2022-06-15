// Created by Ben on 07/03/2022
// Resource: https://www.youtube.com/watch?v=icZj67PTFhc

#include "pch.h"

#include <list>

#include "PathFinder.h"

Engine::PathFinder::PathFinder(TileMap* _tile_map, const AllowedDirections allowed_directions)
	                           : map_width(_tile_map->getWidth()), map_height(_tile_map->getHeight()), tile_map(_tile_map)
{
	buildPathFindingMap(allowed_directions);
}

void Engine::PathFinder::buildPathFindingMap(const AllowedDirections allowed_directions)
{
	// Create an array of the supplied size
	nodes = std::make_unique<Node[]>(map_width * map_height);
	const long long ignore_this = 5; // :)

	for (long long x = ignore_this; x < map_width; x++)
	{
		for (long long y = ignore_this; y < map_height; y++)
		{
			Node* node = findNode(x, y);
			node->x = x;
			node->y = y;
			node->obstacle = tile_map->findTile(x, y)->tile_state == TileMap::Tile::TileState::Water; // Other unwalkable stuff?
			node->parent = nullptr;
			node->vistied = false;

			// Straight Connections
			if (allowed_directions == AllowedDirections::BOTH || allowed_directions == AllowedDirections::STRAIGHT)
			{
				if (y > 0)
					node->neighbours.push_back(&nodes[(y - 1) * map_width + (x + 0)]);
				if (y < map_height - 1)
					node->neighbours.push_back(&nodes[(y + 1) * map_width + (x + 0)]);
				if (x > 0)
					node->neighbours.push_back(&nodes[(y + 0) * map_width + (x - 1)]);
				if (x < map_width - 1)
					node->neighbours.push_back(&nodes[(y + 0) * map_width + (x + 1)]);
			}

			// Diagonal Connections 
			if (allowed_directions == AllowedDirections::BOTH || allowed_directions == AllowedDirections::DIAGONAL)
			{
				if (y > 0 && x > 0)
					node->neighbours.push_back(&nodes[(y - 1) * map_width + (x - 1)]);
				if (y < map_height - 1 && x > 0)
					node->neighbours.push_back(&nodes[(y + 1) * map_width + (x - 1)]);
				if (y > 0 && x < map_width - 1)
					node->neighbours.push_back(&nodes[(y - 1) * map_width + (x + 1)]);
				if (y < map_height - 1 && x < map_width - 1)
					node->neighbours.push_back(&nodes[(y + 1) * map_width + (x + 1)]);
			}
		}
	}
}


std::deque<Engine::PathFinder::Node*> Engine::PathFinder::findPath(const long long start_x, const long long start_y,
	                                                               const long long end_x, const long long end_y,
	                                                               const bool greedy_search)
{
	// Goals
	Node* start_node = findNode(start_x, start_y);
	Node* end_node   = findNode(end_x, end_y);

	// Reset all the nodes to a known state
	for (/*unsigned*/ long long x = 0; x < map_width; x++)
	{
		for (/*unsigned*/ long long y = 0; y < map_height; y++)
		{
			Node* node = findNode(x, y);
			node->vistied = false;
			node->global_goal = INFINITY;
			node->local_goal = INFINITY;
			node->parent = nullptr;
		}
	}

	// Cool lambda functions
	auto distance = [](Node* a, Node* b)
	{
		return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
	};

	auto heuristic = [distance](Node* a, Node* b)
	{
		return distance(a, b);
	};

	// Setup starting conditions
	Node* current_node = start_node;
	start_node->local_goal = 0.0F;
	start_node->global_goal = heuristic(start_node, end_node);
	
	std::list<Node*> untested_nodes;
	untested_nodes.push_back(start_node);
	
	// Actually solve!
	while (!untested_nodes.empty()) // && (greedy_search && current_node != end_node)
	{
		untested_nodes.sort([](const Node* lhs, const Node* rhs) { return lhs->global_goal < rhs->global_goal; });

		while (!untested_nodes.empty() && untested_nodes.front()->vistied)
			untested_nodes.pop_front();

		if (untested_nodes.empty())
			break;

		current_node = untested_nodes.front();
		current_node->vistied = true;

		for (auto neighbour_node : current_node->neighbours)
		{
			if (!neighbour_node->vistied && !neighbour_node->obstacle)
				untested_nodes.push_back(neighbour_node);

			float heuristic_local_goal = current_node->local_goal + distance(current_node, neighbour_node);
			if (heuristic_local_goal < neighbour_node->local_goal)
			{
				neighbour_node->parent = current_node;
				neighbour_node->local_goal = heuristic_local_goal;

				neighbour_node->global_goal = neighbour_node->local_goal + heuristic(neighbour_node, end_node);
			}
		}
	}

	return backtrack(end_node);
}

Engine::PathFinder::Node* Engine::PathFinder::findNode(const long long x, const long long y)
{
	return &nodes[y * map_width + x];
}

std::deque<Engine::PathFinder::Node*> Engine::PathFinder::backtrack(Node* end_node)
{
	std::deque<Node*> path;
	Node* p = end_node;

	while (p->parent != nullptr)
	{
		path.push_back(p->parent);
		p = p->parent;
	}
	if (p->parent == nullptr)
	{
		// std::cout << "node location error" << endl;
	}

	return path;
}
