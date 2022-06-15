// Created by Ben on 28 / 02 / 2022
// Refactor by Kieran on 02 / 05 / 2022

#pragma once

#include <Keyboard.h>
#include <Mouse.h>
#include "StepTimer.h"

/// Data
#include "SceneData.h"

// Things that traditional 'scenes' can inherit from so we can easily switch between them
// Use the ECS for gameobjects and stuff
// This is just so we avoid a big state machine really...
struct DrawData2D;

namespace Engine
{
	class Scene : public std::enable_shared_from_this<Scene> // Allows for easy dependency injection if needed
	{
	public:
		Scene(const GameScenes& game_scene) : sceneData(game_scene), DEFAULT_SCENE_DATA(game_scene) {}
		Scene() = default;
		virtual ~Scene() = default;

		//-----------------------------------! SETTER & GETTER FUNCTIONS !-----------------------------------//
		/// <summary>
		/// Used to reset scene data struct to instantiated const - Use when calling init function of scene which provides
		/// game scene forward and backward transitioning
		/// </summary>
		void setDefaultSceneData() { sceneData = DEFAULT_SCENE_DATA; }
		/// <summary>
		/// Get struct containing all scene related information for current scene
		/// </summary>
		/// <returns>sceneData</returns>
		virtual SceneData getSceneData() const { return sceneData; }
		/// <summary>
		/// Get the sceneData.current_value of the scene, used within scene handler for game state assessing
		/// </summary>
		/// <returns>Enum value for current scene</returns>
		virtual GameScenes getCurrentScene() const { return sceneData.current_scene; }
		/// <summary>
		/// Update sceneData struct by altering scene to change to and updating bool, polled by scene handler
		/// </summary>
		/// <param name="new_scene">| Enum value of new game scene</param>
		void setNewScene(const GameScenes& new_scene);
		/// <summary>
		/// Set pause state of scene
		/// </summary>
		/// <param name="pause">| Pause state for scene</param>
		virtual void setPause(const bool& pause) { sceneData.pause_scene = pause; }
		//-------------------------------------------------------------------------------------------------//

		//-----------------------------------! Base Virtual Functions !-----------------------------------//
		/// <summary>
		/// Init function to create scene elements which provides ability to reload scenes
		/// </summary>
		/// <returns>Instantiation state of components and systems</returns>
		virtual void init() {}
		/// <summary>
		/// Instantiate components required for ECS systems
		/// </summary>
		virtual void initComponents() {}
		/// <summary>
		/// Instatiate systems and register components required for them
		/// </summary>
		virtual void initSystems() {}
		/// <summary>
		/// Instatiate sounds data (Engine::AudioHandler::SoundData) for all sounds for 
		/// </summary>
		virtual void initSounds() {};
		/// <summary>
		/// Called every game update
		/// </summary>
		/// <param name="timer">| Delta time</param>
		virtual void update(DX::StepTimer const& timer) {};
		/// <summary>
		/// Render the scene
		/// </summary>
		/// <param name="timer">| Delta time</param>
		virtual void render(DX::StepTimer const& timer) {};
		/// <summary>
		/// Polled every tick and passed from "InputHandler"
		/// </summary>
		/// <param name="keyboard_data">| DirectX Keyboard Information</param>
		virtual void onKey(const DirectX::Keyboard::KeyboardStateTracker& keyboard_data) {};
		/// <summary>
		/// Polled every tick and passed from "InputHandler"
		/// </summary>
		/// <param name="mouse_data">| DirectX Mouse Information</param>
		virtual void onMouse(const DirectX::Mouse::ButtonStateTracker& mouse_data) {};
		//-------------------------------------------------------------------------------------------------//

	protected:
		SceneData sceneData;
		const SceneData DEFAULT_SCENE_DATA;
	};
}
