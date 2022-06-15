#pragma once

// Made by Tom 07/03/22

namespace Engine 
{

	struct Tile
	{
		Tile() = default;
		~Tile() = default;

		enum TileState {
			Grass,
			Water,
			Mountain,
			Void
		};
		TileState tile_state;
	};
}
