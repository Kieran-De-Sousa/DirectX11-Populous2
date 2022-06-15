// Created by Ben 10/03/2022

#pragma once

#include "SceneHandler.h"

#include "GameData.h"

class SceneHandler;

namespace Engine
{
	/// <summary>
	/// Handles keyboard and mouse input
	/// </summary>
	class InputHandler
	{
	public:
		/// <summary>
		/// Inits the system
		/// </summary>
		/// <param name="_window"> | The game's window from Driver </param>
		/// <param name="_game_data"> | The game's game data from Driver </param>
		/// <param name="_scene_handler"> | The scene handler pointer from Driver </param>
		InputHandler(HWND* _window, GameData* _game_data, SceneHandler* _scene_handler);
		~InputHandler() = default;
		/// <summary>
		/// Ticks the system, polling the keyboard and mouse input 
		/// </summary>
		/// <param name="timer"> | Drivers StepTimer </param>
		void update(DX::StepTimer const& timer);

	private:
		/// <summary>
		/// Ticks keyboard input
		/// </summary>
		void pollKeyBoard();
		/// <summary>
		/// Ticks mouse input 
		/// </summary>
		void pollMouse();

		/// <summary>
		/// Pointer to the keyboard 
		/// </summary>
		std::unique_ptr<DirectX::Keyboard> keyboard;
		/// <summary>
		/// Pointer to the mouse
		/// </summary>
		std::unique_ptr<DirectX::Mouse> mouse;
		
		/// <summary>
		/// The game's window from Driver
		/// </summary>
		HWND* window;
		/// <summary>
		/// The game's game data from Driver
		/// </summary>
		GameData* game_data;
		/// <summary>
		/// The game's SceneHandler from Driver 
		/// </summary>
		SceneHandler* scene_handler;
	};
} // namespace Engine
