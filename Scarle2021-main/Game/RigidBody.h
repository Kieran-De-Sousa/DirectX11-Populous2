// Created by Ben on 22/02/2022

#pragma once

#include <SimpleMath.h>

// A basic RigidBody component

namespace Engine
{
	/// <summary>
	/// Holds the physics data of an entity 
	/// </summary>
	struct RigidBody
	{
		RigidBody() = default;
		~RigidBody() = default;

		DirectX::SimpleMath::Vector3 velocity;
		DirectX::SimpleMath::Vector3 acceleration;
		float gravity_scale = 0;
	};
}
