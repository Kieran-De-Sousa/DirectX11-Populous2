// Created by Ben on 22/02/2022

#pragma once

#include <SimpleMath.h>

// A basic transform component

namespace Engine
{
	/// <summary>
	/// Holds the position of an entity 
	/// </summary>
	struct Transform
	{
		Transform() = default;
		~Transform() = default;

		DirectX::SimpleMath::Vector3 position;
		// Changed to be a Vector2 from float to grant scaling for X and Y
		DirectX::SimpleMath::Vector2 scale = DirectX::SimpleMath::Vector2::One;
		float rotation = 0;
		// Follow the camera
		bool follow_parent = true;
	};
}
