// Made by Tom + Ben 09/03/2022

#pragma once

// System 
#include <map>

// DirectX
#include <SimpleMath.h>

// Project
#include "RenderSystem.h"
#include "Vector2Int.h"

namespace Engine
{
	/// <summary>
	/// A basic tile map composed of Tile structs. 
	/// Rendering is outsourced to the scene's specfic RenderSystem
	/// </summary>
	class TileMap
	{
	public:
		TileMap() = default;
		~TileMap() = default;

		/// <summary>
		/// Stores data about the tile, used in an array to store the whole map
		/// </summary>
		struct Tile
		{
			Tile() = default;
			~Tile() = default;

			int x;					   // Tile position x
			int y;					   // Tile posiiton y
			int tile_height = 0;       // The height of the tile, accordant to the tile_state
			enum class TileState {
				Grass,
				Water,
				Mountain1,
				Mountain2,
				Mountain3,
				NonWalkable
				// Any other states here
			};
			TileState tile_state;	    // The state of the tile, used for rendering
			Engine::Entity tile_entity; // The entity that tile is attached to, used for updating its sprite
		};

		/// <summary>
		/// Randomly generates a tilemap (currently not very well..)
		/// </summary>
		/// <param name="_width"> | The x size of the tile map </param>
		/// <param name="_width"> | The y size of the tile map </param>
		void generateFromRandom(const unsigned long long _width, const unsigned long long _height);
		/// <summary>
		/// Creates a tilemap from a premade file
		/// </summary>
		/// <param name="_file"> | The file path of tile to be loaded </param>
		void generateFromFile(char _file);
		/// <summary>
		/// Draws the tile map and registers the created sprites with the RenderSystem
		/// </summary>
		/// <param name="coordinator"> | Reference to the ecs system on the scene </param>
		/// <param name="render_system"> | Reference to the render system on the scene </param>
		/// <param name="tile_textures"> | A map of TileState's to file paths to allow for easy texture customisation </param>
		/// <param name="_tile_size"> | The size of a single tile (x and y) </param>
		/// <param name="tile_scale"> | The scale of a single tile (x and y seperate) </param>
		/// <param name="tile_rotation"> | The rotation of a tile, can be used to rotate the entire map </param>
		/// <param name="_map_offset"> | How far off (0, 0) the map will be drawn </param>
		void drawMap(Engine::Coordinator* coordinator, Engine::RenderSystem* render_system,
			         const std::map<Tile::TileState, std::string>& tile_textures, const DirectX::SimpleMath::Vector2& _tile_size,
					 const DirectX::SimpleMath::Vector2& tile_scale, float tile_rotation, const DirectX::SimpleMath::Vector2& _map_offset);

		/// <summary>
		/// Works out what the surrounding height of the tiles around the tile at worldToTile(x, y) is
		/// </summary>
		/// <returns> A vector of the surrounding tile hieghts </returns>
		vector<int> resolveTiles(DirectX::SimpleMath::Vector3 position);

		/// <summary>
		/// Updates the clicked tile by increment * 2, and the 8 the surrounding tiles by increment
		/// </summary>
		/// <param name="position"> | The world position, will use worldToTile(x, y) to work out the clicked tile </param>
		/// <param name="render_system"> | Reference to the scenes render system </param>
		/// <param name="tile_textures"> | Map of TileState - file path for easy flexibility when changing tile sprites </param>
		/// <param name="increment"> | How much the tiles will be raised up/down </param>
		void changeTiles(DirectX::SimpleMath::Vector3 position, Engine::RenderSystem* render_system, const std::map<Tile::TileState, std::string>& tile_textures, int increment);
		/// <summary>
		/// Updates the clicked tile by increment, and all surrounding tiles recursively until tile->tile_height + 1 < parent_tile.tile_height
		/// (Parent tile being the center of the surrounding tiles)
		/// </summary>
		/// <param name="parent_tile"> | The tile from which the recurive raise / lower will spread </param>
		/// <param name="render_system"> | Reference to the scenes render system </param>
		/// <param name="tile_textures"> | Map of the TileState's to file paths to allow for easy texture customisation </param>
		/// <param name="increment"> | How much the tiles will move up / down </param>
		/// <param name="first_tile"> | Used to control the recursion!! No touchies </param>
		void changeTilesRecursive(const Tile& parent_tile, const std::shared_ptr<Engine::RenderSystem> render_system,
			                      const std::map<Tile::TileState, std::string>& tile_textures, int increment, bool first_tile=true);

		/// <summary>
		/// Converts tile cordinate to a world space vector
		/// </summary>
		/// <param name="tile_cords"> | The x and y position of the tile </param>
		/// <returns> A world space vector3 of the tile positiion </returns>
		DirectX::SimpleMath::Vector3 tileToWorld(const Engine::Vector2Int& tile_cords);
		/// <summary>
		/// Converts world space vector into a tile cordinate
		/// </summary>
		/// <param name="world_cords"> | The world space vector </param>
		/// <returns> A tile map coordinate as a Vector2Int of world space vector </returns>
		Engine::Vector2Int worldToTile(const DirectX::SimpleMath::Vector3& world_cords);

		/// <summary>
		/// Converts a 2d coord (x/y) into a 1d index and returns the tile at that position
		/// </summary>
		/// <param name="x"> | The tile cord x position </param>
		/// <param name="y"> | The tile cord y position </param>
		/// <returns> A pointer to the Tile at the specified posiiton </returns>
		Tile* findTile(unsigned long long x, unsigned long long y);

		/// <summary>
		/// Returns the width of the map
		/// </summary>
		/// <returns> The x size of the map </returns>
		unsigned long long getWidth() const;
		/// <summary>
		/// Returns the height of the map
		/// </summary>
		/// <returns> The y size of the map </returns>
		unsigned long long getHeight() const;

		/// <summary>
		/// Returns size of the tile (taking to account scale)
		/// </summary>
		/// <returns> A vector2 for the size of the tile </returns>
		DirectX::SimpleMath::Vector2 getTileSize() const;
		/// <summary>
		/// Returns the offset at which the map is rendered
		/// </summary>
		/// <returns> A vector2 for the map offset </returns>
		DirectX::SimpleMath::Vector2 getMapOffset() const;

		/// <summary>
		/// Returns the tile map array
		/// </summary>
		/// <returns> A C style array wrapped in a unique pointer containing all of the tile objects </returns>
		Tile* getMap() const;

	private:
		/// <summary>
		/// A helper function for changeTilesRecursive. Checks if the increment update is valid and if so updates the tile sprites 
		/// </summary>
		/// <returns> A vector2 for the map offset </returns>
		void changeAllTilesRecursiveHelper(const Engine::Vector2Int& tile, const std::shared_ptr<Engine::RenderSystem> render_system,
										   const std::map<Tile::TileState, std::string>& tile_textures, int increment);

		/// <summary>
		/// The tilemap (its a single dimensional array, but a smartpointer so we don't have to manage it.
		/// This is preferable over a 2d array as its more memory efficient... apparently...)
		/// </summary>
		std::unique_ptr<Tile[]> tiles = nullptr;

		/// <summary>
		/// The x size of the map
		/// </summary>
		unsigned long long width = 0;
		/// <summary>
		// The y size of the map
		/// </summary>
		unsigned long long height = 0;
		/// <summary>
		/// The size of the tile sprite
		/// </summary>
		DirectX::SimpleMath::Vector2 tile_size;
		/// <summary>
		/// The offset of the map (how far +/- (0, 0) on the x/y)
		/// </summary>
		DirectX::SimpleMath::Vector2 map_offset;
	};
} // namespace Engine
