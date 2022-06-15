#pragma once

#include <vector>

enum class GameScenes : int
{
	UNDEFINED = 0,
	MAIN_MENU = 1,
	GAME	  = 2
};

/// Provides enum iteration if required (can be used for factory method?)
static const std::vector<GameScenes> ALL_GAME_SCENES{ GameScenes::UNDEFINED,
											 GameScenes::MAIN_MENU,
											 GameScenes::GAME};

struct SceneData
{
	explicit SceneData(GameScenes scene_type) : current_scene(scene_type) {}
	SceneData() = default;
	virtual ~SceneData() = default;

	GameScenes current_scene = GameScenes::UNDEFINED;
	GameScenes new_scene = GameScenes::UNDEFINED;
	bool change_scene = false;
	bool pause_scene = false;
	bool exit_game = false;
};