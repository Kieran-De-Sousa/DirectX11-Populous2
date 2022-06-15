// Created by Ben on 28/02/2022
// Refactor by Kieran on 02 / 05 / 2022

#pragma once

/// Base Scene Class ///
#include "Scene.h"

// Could just as easily be called 'StateHandler' or something
// Unlike in bcw, scenes and gameobjects aren't tied together in this project

struct DrawData2D;

namespace Engine
{
	class SceneHandler
	{
	public:
		SceneHandler();
		~SceneHandler() = default;

		SceneHandler(const SceneHandler&) = delete;
		SceneHandler& operator=(const SceneHandler&) = delete;

#pragma region Kieran Scene Handler
		/// <reference> This method of scene handling has a similiar implementation to my "More Games In C++" version, I chose to use this as
		/// <reference> I found the method for scene transitioning and handling effective. Compared to that implementation, this one is simpler
		/// <reference> due to requiring less scenes for the "vertical slice" although the system is easily expandable when being handed off to
		/// <reference> another team to continue. This applies for the Scene base class and the SceneData struct. - Kieran

		/// <summary>
		/// Called on scene handler instantiation - Loops through vector containing all enum values for scenes
		/// and calls 'sceneFactory' method to create and push a scene to back of 'all_scenes' vector.
		/// </summary>
		void createScenes();
		/// <summary>
		/// Gets the data from the current scene and assesses whether a scene transition is requested
		/// </summary>
		void checkCurrentSceneState();
		/// <summary>
		/// Pass in shared ptr to make the current scene
		/// </summary>
		/// <param name="new_scene">| Enum type for game scene</param>
		void setCurrentScene(const std::shared_ptr<Scene>& new_scene);
		/// <summary>
		/// Returns pointer to current scene
		/// </summary>
		/// <returns>| Pointer to current scene</returns>
		Scene* getCurrentScene();
		/// <summary>
		/// Create every game scene through enum iteration. Returns a nullptr when a switch case isn't provided for the
		/// scene to instantiate which is filtered out within the 'CreateScenes' function.
		/// </summary>
		/// <param name="scene_type">| Enum value of scene</param>
		/// <returns>| Pointer to scene attempted to create</returns>
		std::shared_ptr<Engine::Scene> sceneFactory(const GameScenes& scene_type);
		/// <summary>
		/// Add a scene to the vector of all scenes
		/// </summary>
		/// <param name="scene">| Pointer of scene</param>
		void addScene(const std::shared_ptr<Scene>& scene);
		/// <summary>
		/// Possibly unneeded method of exitting game. Checked every update to see if current scene
		/// wishes to exit the game, checked with Game.cpp and handled accordingly
		/// </summary>
		/// <returns>exit state for current scene</returns>
		bool onExit() const { return exit_game; }

		////! Calls virtual functions within current scene ////!
		virtual void update(DX::StepTimer const& timer);
		virtual void render(DX::StepTimer const& timer);
#pragma endregion

// Depreciated method of scene handling - See above
#pragma region Ben Scene Handler
		/*
		template<typename T>
		std::shared_ptr<T> changeScene()
		{
			std::shared_ptr<T> scene = std::make_shared<T>();
			changeScene(scene);
			return scene;
		}
		void changeScene(const std::shared_ptr<Scene>& new_scene);

		bool getPausedState();
		void setPausedState(bool _paused);
		*/
#pragma endregion

	protected:
		// Scene
		std::shared_ptr<Scene> current_scene;
		std::vector<std::shared_ptr<Scene>> all_scenes;

		// Data within scenes
		SceneData current_scene_data;
		bool exit_game = false;
		bool paused = false;
	};
}
