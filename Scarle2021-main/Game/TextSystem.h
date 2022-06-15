// Created by Kieran on 21/03/2022

#pragma once

#include "DrawData2D.h"
#include "CommonStates.h"
#include "StepTimer.h"

#include "System.h"
#include "Coordinator.h"

/// Text rendering system
// Helpful Resources: https://github.com/microsoft/DirectXTK/wiki/SpriteFont
namespace Engine
{
	/// <summary>
	/// Handles rendering, initialisation, and all other operations regarding text
	/// </summary>
	class TextSystem : public Engine::System
	{
	public:
		TextSystem();
		~TextSystem() = default;

		/// <summary>
		/// Creates link to ECS coordinator and establishes link to shared draw data information
		/// </summary>
		/// <param name="_coordinator">| Pointer to the scene's coordinator</param>
		void init(Coordinator * _coordinator);
		/// <summary>
		/// Update the system, rendering all text entities
		/// </summary>
		void update();

		/// <summary>
		/// Initialise text with the given string
		/// </summary>
		/// <param name="_text">| Text to display</param>
		/// <param name="entity">| Entity with text to init</param>
		void initText(const std::string& _text, const Entity& entity);
		/// <summary>
		/// Method to create fonts to provide text their own font type, currently unused due to being unable
		/// to create the font data
		/// </summary>
		/// <param name="_filename">| Filename of font</param>
		/// <returns>SpriteFont pointer</returns>
		SpriteFont* initFont(const std::string& _filename);

	private:
		// ECS 
		Coordinator* coordinator;
		DrawData2D* draw_data;
		CommonStates* common_states;
		ID3D11Device1* d3d_device;
	};
}