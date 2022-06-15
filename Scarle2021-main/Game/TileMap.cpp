// Made by Tom + Ben 09/03/2022

#include "pch.h"

// System
#include <cassert>
#include <iostream>

// Project
#include "MoreMath.h"

#include "Transform.h"
#include "SpriteRenderer.h"

// Header
#include "TileMap.h"


void Engine::TileMap::generateFromRandom(const unsigned long long _width, const unsigned long long _height)
{
    // Create the tilemap array
    tiles = std::make_unique<Tile[]>(_width * _height);

    // Assign constant values
    width = _width;
    height = _height;

    // Setup each tile in the tile array
    for (unsigned long long x = 0; x < width; x++)
    {
        for (unsigned long long y = 0; y < height; y++)
        {
            int random = rand() % 10 + 1;
            Tile* tile = findTile(x, y);
            tile->x = x;
            tile->y = y;
            if (random < 3)
            {
                tile->tile_height = 0;
                tile->tile_state = Tile::TileState::Water;
            }
            else if (random == 8)
            {
                tile->tile_height = 2;
                tile->tile_state = Tile::TileState::Mountain1;
            }
            else if (random == 9)
            {
                tile->tile_height = 3;
                tile->tile_state = Tile::TileState::Mountain2;
            }
            else if (random == 10)
            {
                tile->tile_height = 4;
                tile->tile_state = Tile::TileState::Mountain3;
            }
            else
            {
                tile->tile_height = 1;
                tile->tile_state = Tile::TileState::Grass;
            }
        }
    }
}

void Engine::TileMap::generateFromFile(char _file) // char?? std::string >:(    
{
    // Init tiles array with width * height
    // Loop through file, use the findTile(x, y) function to return the tile you want to setup

    /* Could do something like..:
      std::vector<std::string> lines;

      if (std::ifstream(file_path))
      {
        std::ifstream file_stream;
        file_stream.open(file_path);

        std::string line;
        while (std::getline(file_stream, line))
        {
          lines.push_back(line);
        }

        file_stream.close();
      }

      [now you have a vector of all the lines in the file, could iterate through them]
    */
}

void Engine::TileMap::drawMap(Engine::Coordinator* coordinator, Engine::RenderSystem* render_system,
                              const std::map<Tile::TileState, std::string>& tile_textures, const DirectX::SimpleMath::Vector2& _tile_size,
                              const DirectX::SimpleMath::Vector2& tile_scale, const float tile_rotation, const DirectX::SimpleMath::Vector2& _map_offset)
{
    // Check that the tilemap array isn't nullptr. If it is, then we couldn't draw anything anyway!
    assert(tiles != nullptr && "You must initialize a tilemap before you try and draw it!");

    // Assign constant variables
    tile_size = _tile_size;
    map_offset = _map_offset;

    // Loop through every tile and create a sprite for it
    for (long long x = 0; x < width; x++)
    {
        for (long long y = 0; y < height; y++)
        {
            Engine::Entity new_entity = coordinator->createEntity();
            Tile* tile = findTile(x, y);

            coordinator->addComponent<Engine::Transform>(
                new_entity,
                { 
                    DirectX::SimpleMath::Vector3(map_offset.x + ((x - y) * tile_size.x / 2), map_offset.y + ((x + y) * tile_size.y / 2), 0),
                    tile_scale, 
                    tile_rotation 
                });
                coordinator->addComponent<Engine::SpriteRenderer>(
                    new_entity, {});

            render_system->initSprite(tile_textures.at(tile->tile_state), new_entity);
            tile->tile_entity = new_entity;
        }
    }
}

vector<int> Engine::TileMap::resolveTiles(DirectX::SimpleMath::Vector3 position)
{
    // Calculate the selected tile
    vector<int> neighbourTiles{};
    auto tileLocal = worldToTile(position);

    // Calculate the selected tiles x, y and reference
    int x = tileLocal.x;
    int y = tileLocal.y;
    Tile& currentTile = tiles[(tiles[y * width + x].y) * width + (tiles[y * width + x].x)];

    // Calculate the surrounding 4 tiles (N, S, E, W) heights and return them in a vector of ints
    Tile& topLeft = tiles[(tiles[y * width + x].y) * width + (tiles[y * width + x].x - 1)];
    Tile& topRight = tiles[(tiles[y * width + x].y - 1) * width + (tiles[y * width + x].x)];
    Tile& botLeft = tiles[(tiles[y * width + x].y) * width + (tiles[y * width + x].x + 1)];
    Tile& botRight = tiles[(tiles[y * width + x].y + 1) * width + (tiles[y * width + x].x + 1)];

    // std::cout << topLeft.tile_height << " " << topRight.tile_height << " " << botLeft.tile_height << " " << botRight.tile_height;
    return { topLeft.tile_height, topRight.tile_height, botLeft.tile_height, botRight.tile_height };
}

void Engine::TileMap::changeTiles(DirectX::SimpleMath::Vector3 position, Engine::RenderSystem* render_system, const std::map<Tile::TileState, std::string>& tile_textures, int increment)
{
    // Calculate the 8 surrounding tiles
    // Also add the intial tile to the vector so it gets incremented
    Engine::Vector2Int tile = worldToTile(position);
    std::vector<Engine::Vector2Int> tiles_to_effect = {
        tile, /*tile,*/ // Twice so it gets incremented twice
        { tile.x + 1, tile.y },
        { tile.x - 1, tile.y },
        { tile.x, tile.y + 1 },
        { tile.x, tile.y - 1 }, 
        { tile.x + 1, tile.y  + 1},
        { tile.x - 1, tile.y - 1},
        { tile.x + 1, tile.y - 1},
        { tile.x - 1, tile.y + 1},
    };

    // Loop through neighbour tiles and change their height by the increment 
    for (auto& local_tile : tiles_to_effect)
    {
        int x = local_tile.x;
        int y = local_tile.y;

        if (x >= 0 && y >= 0 && x < width && y < height) 
        {
            tiles[y * width + x].tile_height += increment;
            if (tiles[y * width + x].tile_height < 1)
            {
                tiles[y * width + x].tile_state = Tile::TileState::Water;
            }
            else if (tiles[y * width + x].tile_height == 1)
            {
                tiles[y * width + x].tile_state = Tile::TileState::Grass;
            }
            else if (tiles[y * width + x].tile_height == 2)
            {
                tiles[y * width + x].tile_state = Tile::TileState::Mountain1;
            }
            else if (tiles[y * width + x].tile_height == 3)
            {
                tiles[y * width + x].tile_state = Tile::TileState::Mountain2;
            }
            else if (tiles[y * width + x].tile_height == 4)
            {
                tiles[y * width + x].tile_state = Tile::TileState::Mountain3;
            }

            // Update the tiles sprite via the render system
            render_system->updateSprite(tile_textures.at(tiles[y * width + x].tile_state), tiles[y * width + x].tile_entity);
        }
    }
}

void Engine::TileMap::changeTilesRecursive(const Tile& parent_tile, const std::shared_ptr<Engine::RenderSystem> render_system,
                                           const std::map<Tile::TileState, std::string>& tile_textures, const int increment, const bool first_tile)
{
    // The 8 tiles surrounding the parent tile
    std::vector<Engine::Vector2Int> neighbours = {
        { parent_tile.x + 1, parent_tile.y },
        { parent_tile.x, parent_tile.y + 1 },
        { parent_tile.x - 1, parent_tile.y },
        { parent_tile.x, parent_tile.y - 1 },
        { parent_tile.x + 1, parent_tile.y + 1 },
        { parent_tile.x - 1, parent_tile.y - 1 },
        { parent_tile.x + 1, parent_tile.y - 1 },
        { parent_tile.x - 1, parent_tile.y + 1 },
    };

    // If the tile is the first_tile, increment is to start of the recursion
    if (first_tile)
    {
        changeAllTilesRecursiveHelper({ parent_tile.x, parent_tile.y }, render_system, tile_textures, increment);
    }

    // Loop through all the neighbour tiles, calling changeAllTilesRecursiveHelper and changeTilesRecursive on all valid tiles
    // A valid tile is one which is not outside the bounds of the map
    for (const auto& tile : neighbours)
    {
        if (!(tile.x >= 0 && tile.y >= 0 && tile.x < width && tile.y < height)) return;

        Engine::TileMap::Tile* map_tile = findTile(tile.x, tile.y);

        if (map_tile->tile_height + 1 < parent_tile.tile_height)
        {
            changeAllTilesRecursiveHelper({ map_tile->x, map_tile->y }, render_system, tile_textures, increment);
            changeTilesRecursive(*map_tile, render_system, tile_textures, increment, false);
        }
    }
}

DirectX::SimpleMath::Vector3 Engine::TileMap::tileToWorld(const Engine::Vector2Int& tile_cords)
{
    return {(tile_cords.x - tile_cords.y) * (tile_size.x /2) + map_offset.x, (tile_cords.x + tile_cords.y) * (tile_size.y / 2) + map_offset.y,
             0}; // Just in case we want to do something with height
}

Engine::Vector2Int Engine::TileMap::worldToTile(const DirectX::SimpleMath::Vector3& world_cords)
{
    // std::cout << "Mouse pos: " << world_cords.x << ", " << world_cords.y << endl;

    // Calculate the position of the tile based off world coords taking into account map offset
    Vector2Int changedTile = { 0,0 };
    changedTile.x = (0.5f + ((world_cords.x - map_offset.x) / (tile_size.x / 2) + (world_cords.y - map_offset.y) / (tile_size.y / 2)) / 2);
    changedTile.y = ((0.5f + (((world_cords.y - map_offset.y) / (tile_size.y / 2)) - (world_cords.x - map_offset.x) / (tile_size.x / 2)) / 2));

    // Cast from float to int (automatically rounding up by 1) and return the vector
    changedTile.x = static_cast<int>(changedTile.x);
    changedTile.y = static_cast<int>(changedTile.y);

    // std::cout << changedTile.x << " " << changedTile.y << endl;
    return changedTile;
}

Engine::TileMap::Tile* Engine::TileMap::findTile(const unsigned long long x, const unsigned long long y)
{
    return &tiles[y * width + x];
}

unsigned long long Engine::TileMap::getWidth() const
{
    return width;
}

unsigned long long  Engine::TileMap::getHeight() const
{
    return height;
}

DirectX::SimpleMath::Vector2 Engine::TileMap::getTileSize() const
{
    return tile_size;
}

DirectX::SimpleMath::Vector2 Engine::TileMap::getMapOffset() const
{
    return map_offset;
}

Engine::TileMap::Tile* Engine::TileMap::getMap() const
{
    return tiles.get();
}

void Engine::TileMap::changeAllTilesRecursiveHelper(const Engine::Vector2Int& cord, const std::shared_ptr<Engine::RenderSystem> render_system, 
                                                    const std::map<Tile::TileState, std::string>& tile_textures, const int increment)
{
    // Get the tile from the passed in cordinates
    Tile* map_tile = findTile(cord.x, cord.y);

    // Increment or decrement the tile height and work out if the new height is valid
    const int MIN_HEIGHT = 0;
    const int MAX_HEIGHT = 4;

    map_tile->tile_height += increment;
    //map_tile->TileHeight = HelperMethods::MoreMath::clamp(map_tile->TileHeight, MIN_HEIGHT, MAX_HEIGHT); ??
    if (map_tile->tile_height > MAX_HEIGHT) map_tile->tile_height = MAX_HEIGHT;
    if (map_tile->tile_height < MIN_HEIGHT) map_tile->tile_height = MIN_HEIGHT;

    // Choose the correct tile sprite based on the new tile height
    switch (map_tile->tile_height)
    {
    case 0:
        map_tile->tile_state = Tile::TileState::Water;
        break;
    case 1:
        map_tile->tile_state = Tile::TileState::Grass;
        break;
    case 2:
        map_tile->tile_state = Tile::TileState::Mountain1;
        break;
    case 3:
        map_tile->tile_state = Tile::TileState::Mountain2;
        break;
    case 4:
        map_tile->tile_state = Tile::TileState::Mountain3;
        break;
    default:
        // panic
        break;
    }

    // Update the sprite via the render system
    render_system->updateSprite(tile_textures.at(map_tile->tile_state), map_tile->tile_entity);
}
