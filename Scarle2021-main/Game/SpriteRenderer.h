// Slight refactor/expansion by Kieran on 17/03/2022
// NOTE: Would rename this file to "GameObject2DRenderer" 
// but can't rename header globally and affect all files - Kieran

#pragma once

#include <unordered_map>
#include <vector>
#include <array>

#include <SpriteBatch.h>
#include <SimpleMath.h>
#include <string>

// For TextRenderer component
#include "SpriteFont.h"

namespace Engine
{
	// Created by Ben on 28/03/2022
	struct Animator
	{
		Animator() = default;
		~Animator() = default;

		bool in_use = false;

		std::unordered_map<std::string, std::vector<std::array<LONG, 4>>> animations;
		double tick_speed;
		double clock;

		size_t current_frame;
		std::string current_animation;
	};

	struct GameObject2DRenderer
	{
		GameObject2DRenderer() = default;
		~GameObject2DRenderer() = default;

		DirectX::SimpleMath::Color colour;
		DirectX::SimpleMath::Vector2 origin;

		int z_order = 1;
		bool move_with_camera = true;
	};

	// Created by Ben on 28/02/2022
	// A basic sprite component
	struct SpriteRenderer : public GameObject2DRenderer
	{
		SpriteRenderer() = default;
		~SpriteRenderer() = default;
	
		// Sprite
		ID3D11ShaderResourceView* texture;
		RECT* source_rect; // For sprite sheet animation 
		DirectX::SimpleMath::Vector2 texture_dimensions;
		DirectX::SpriteEffects sprite_effects;

		// Animation
		Animator animator;
	};

	// Created by Kieran on 17/03/2022
	// A basic text component
	struct TextRenderer : public GameObject2DRenderer
	{
		TextRenderer() = default;
		~TextRenderer() = default;

		std::string font_filename;
		DirectX::SpriteFont* font;

		std::string text;
	};
}
