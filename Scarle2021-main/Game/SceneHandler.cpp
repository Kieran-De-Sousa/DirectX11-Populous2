// Created by Ben on 28/02/2022
// Refactor by Kieran on 02 / 05 / 2022

#include "pch.h"

#include "Driver.h"
#include "SceneHandler.h"

/// <note> Declared here due to circular dependancy issues - Kieran (Thank you Ben)
/// ALL GAME SCENES ///
#include "SceneMainMenu.h"
#include "SceneGame.h"

Engine::SceneHandler::SceneHandler()
{
	createScenes();
}

#pragma region Kieran Scene Handler
/// <reference> This method of scene handling has a similiar implementation to my "More Games In C++" version, I chose to use this as
/// <reference> I found the method for scene transitioning and handling effective. Compared to that implementation, this one is simpler
/// <reference> due to requiring less scenes for the "vertical slice" although the system is easily expandable when being handed off to
/// <reference> another team to continue. - Kieran

void Engine::SceneHandler::setCurrentScene(const std::shared_ptr<Scene>& new_scene)
{
	current_scene = new_scene;
	current_scene->init();
}

std::shared_ptr<Engine::Scene>Engine::SceneHandler::sceneFactory(const GameScenes& scene_type)
{
	switch (scene_type)
	{
		case GameScenes::MAIN_MENU:
		{
			return std::make_shared<SceneMainMenu>(scene_type);
		}
		case GameScenes::GAME:
		{
			return std::make_shared<SceneGame>(scene_type);
		}
		default:
		{
			return nullptr;
		}
	}
}

void Engine::SceneHandler::addScene(const std::shared_ptr<Scene>& scene)
{
	all_scenes.push_back(scene);
}

void Engine::SceneHandler::createScenes()
{
	for (const auto& sceneType : ALL_GAME_SCENES)
	{
		auto scene = sceneFactory(sceneType);
		if (scene != nullptr)
		{
			addScene(scene);
		}
	}
	/// Set the current scene to first scene created from scene factory
	setCurrentScene(all_scenes.front());
}

void Engine::SceneHandler::checkCurrentSceneState()
{
	/// Grab the current scenes data
	current_scene_data = current_scene->getSceneData();

	/// Assess whether the current scene wishes to change scene
	if (current_scene_data.change_scene)
	{
		/// Loop through vector containing shared pointers to all scenes and assess whether their enum type is the same
		/// as the requested new scene
		for (const auto& scene : all_scenes)
		{
			if (scene->getCurrentScene() == current_scene_data.new_scene)
			{
				setCurrentScene(scene);
			}
		}
	}
}

Engine::Scene* Engine::SceneHandler::getCurrentScene()
{
	return current_scene.get();
}
#pragma endregion

void Engine::SceneHandler::update(DX::StepTimer const& timer)
{
	checkCurrentSceneState();
	current_scene->update(timer);
}

void Engine::SceneHandler::render(DX::StepTimer const& timer)
{
	current_scene->render(timer);
}

#pragma region Ben Scene Handler
/*void Engine::SceneHandler::changeScene(std::shared_ptr<Scene> const& new_scene)
{
	current_scene = new_scene;
}

bool Engine::SceneHandler::getPausedState()
{
	return paused;
}

void Engine::SceneHandler::setPausedState(const bool _paused)
{
	paused = _paused;
}
*/
#pragma endregion