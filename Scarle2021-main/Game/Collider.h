// Created by Ben 15/03/2022

#pragma once

// System
#include <functional>

// DirectX
#include <SimpleMath.h>

// Project 
#include "Types.h"

namespace Engine
{
	/// <summary>
	/// Holds collision infomation 
	/// </summary>
	struct Collider
	{
		Collider() = default;
		~Collider() = default;

		// Allows for a collider to be created without any function callbacks for onCollisionEnter/Exit
		void empty_template(const Engine::Entity&, const Engine::Entity&) {};
		
		std::function<void(const Engine::Entity&, const Engine::Entity&)> on_collision_enter = [this](Entity, Entity) {empty_template(NULL_ENTITY, NULL_ENTITY); };
		std::function<void(const Engine::Entity&, const Engine::Entity&)> on_collision_exit = [this](Entity, Entity) {empty_template(NULL_ENTITY, NULL_ENTITY); };

		DirectX::SimpleMath::Vector2 collider_size;
		bool should_collide = true;

		// void (*on_collision_enter)(/*Engine::Entity*/);
		// void (*on_collision_exit)(/*Engine::Entity*/);
		// ^ Couldn't get this to work generically 
	};
}
