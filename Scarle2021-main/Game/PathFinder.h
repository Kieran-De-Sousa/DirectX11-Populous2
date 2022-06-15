// Created by Ben on 07/03/2022
// Resource: https://www.youtube.com/watch?v=icZj67PTFhc

#pragma once

// System
#include <vector>
#include <deque>

// Project 
#include "TileMap.h"

namespace Engine
{
	/// <summary>
	/// A basic path finder that works off a 1D array and uses A* to calculate the shortest route between two points
	/// </summary>
	class PathFinder
	{
	public:
		
		/// <summary>
		/// The links that will be built up on the pathfinder between nodes.
		/// Straight - N, S, E, W
		/// Diagonal - NE, SE, NW, SW
		/// Both - All cardinal directions
		/// </summary>
		enum class AllowedDirections { BOTH, STRAIGHT, DIAGONAL };
		/// <summary>
		/// Contructs the pathfinding map based off the _tile_map* and allowed_directions
		/// </summary>
		/// <param name="_tile_map"> | The tile map to base the path finding map off of </param>
		/// <param name="allowed_directions"> | The links that will be built up on the pathfinder map between nodes </param>
		PathFinder(TileMap* _tile_map, AllowedDirections allowed_directions = AllowedDirections::BOTH);
		~PathFinder() = default;

		/// <summary>
		/// What the pathfinding map is made up of 
		/// </summary>
		struct Node
		{
			bool obstacle = false;          // Can an agent path through this node?

			bool vistied = false;           // Has the node been visited on shortest path search?
			float global_goal;              // Used in the shortest path search, heuristic between the start and this node
			float local_goal;               // Used in the shortest path search, heuristic between the tested node and this node
			int x;                          // The x position of the node
			int y;                          // The y position of the node
			std::vector<Node*> neighbours;  // The nodes neighbours (based off the AllowedDirections)
			PathFinder::Node* parent;       // The nodes parent, used in the shortest path search
		};

		/// <summary>
		/// Creates the links (assigns the neighbours) between the nodes on the pathfinding map
		/// </summary>
		/// <param name="allowed_directions"> The direction of the links build up between pathfinding nodes </param>
		void buildPathFindingMap(AllowedDirections allowed_directions);
		/// <summary>
		/// Finds the shortest path between (start_x, start_y) and (end_x, end_y)
		/// </summary>
		/// <param name="start_x"> | The x cord in tilemap space the path will be calcuated from </param>
		/// <param name="start_y"> | The y cord in tilemap space the path will be calcuated from</param>
		/// <param name="end_x"> | The x cord in tilemap space that the path will be calculated to</param>
		/// <param name="end_y"> | The y cord in tilemap space that the path will be calculated to</param>
		/// <param name="greedy_search"> | Should the path take a greedy option to reach the end goal? Or calcuate the true shortest path </param>
		/// <returns> A deque of Nodes* containing the shortest path ordered from start to end </returns>
		std::deque<Node*> findPath(long long start_x, long long start_y,
			                       long long end_x,   long long end_y,
			                       bool greedy_search = false);

		/// <summary>
		/// The pathfinder map is based off a 1D array for efficiency, this function returns a pointer to the node and the supplied x and y coords
		/// </summary>
		/// <param name="x"> | The x coordinate of the node </param>
		/// <param name="y"> | The y coordinate of the node </param>
		/// <returns> The node at the supplied x, y coord </returns>
		Node* findNode(long long x, long long y);
	private:
		/// <summary>
		/// Backtracks from the last node (destination node) in the path and builds up a route based on those node's parents 
		/// </summary>
		/// <param name="end_node"> | The last node in the path </param>
		/// <returns> A deque of all the nodes in the shortest path </returns>
		std::deque<Node*> backtrack(Node* end_node);

		/// <summary>
		/// The pathfinding map
		/// </summary>
		std::unique_ptr<Node[]> nodes;
		/// <summary>
		/// The maps x size
		/// </summary>
		long long map_width;
		/// <summary>
		/// The maps y size
		/// </summary>
		long long map_height;

		/// <summary>
		/// Reference to the tile map for intial creation and updating nodes
		/// </summary>
		TileMap* tile_map;
	};

	// We use a deque so that the user can do whatever they want immediately with the path data
} // namespace Engine
