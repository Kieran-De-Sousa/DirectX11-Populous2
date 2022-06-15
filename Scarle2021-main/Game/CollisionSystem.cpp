// Created by Ben 15/03/2022

#include "pch.h"

#include <iostream>

#include "Transform.h"
#include "Collider.h"

#include "CollisionSystem.h"

Engine::CollisionSystem::CollisionSystem() : coordinator(nullptr)
{
	// Can't initialise variables in here because of header only circular dependency issues
}

void Engine::CollisionSystem::init(Coordinator* _coordinator)
{
	coordinator = _coordinator;
}

void Engine::CollisionSystem::update(const DX::StepTimer& timer)
{
	for (auto& x : entities)
	{
		for (auto& y : entities) // this isnt doing the faster collision anymore...
		{
			if (x == y) continue;

			Collider& col_one = coordinator->getComponent<Engine::Collider>(x);
			Collider& col_two = coordinator->getComponent<Engine::Collider>(y);

			if (!col_one.should_collide || !col_two.should_collide) continue;

			if (checkAABB(x, y))
			{
				col_one.on_collision_enter(x, y);
				col_two.on_collision_enter(y, x);
			}

			// If need onCollisionExit, then need a vector of all current collisions and check 
			// then in / out (like the NotMario game)
		}
	}
}

void Engine::CollisionSystem::initCollider(const std::function<void(const Engine::Entity&, const Engine::Entity&)>& _on_collision_enter, const std::function<void(const Engine::Entity&, const Engine::Entity&)>& _on_collision_exit,
	                                       const DirectX::SimpleMath::Vector2& collider_size, const Engine::Entity& entity)
{
	Collider& collider = coordinator->getComponent<Engine::Collider>(entity);
	collider.on_collision_enter = _on_collision_enter;
	collider.on_collision_exit = _on_collision_exit;
	collider.collider_size = collider_size;

	/* Example of collider initialization:
		collision_system->initCollider([this](const Engine::Entity& collision) {memberOnCollisionEnterMethod(collision); },
                                       [this](const Engine::Entity& collision) {memberOnCollisionExitMethod(collision);  }, 
                                       collder_size, unit);
	*/
}

void Engine::CollisionSystem::deinitColliderAtEndOfFrame(const Engine::Entity& entity, bool delete_entity_as_well)
{
	std::cout << "The function [deinitColliderAtEndOfFrame] is deprecated, you can remove it from your code!" << std::endl;
}

void Engine::CollisionSystem::deinitCollider(const Engine::Entity& entity)
{
	std::cout << "The function [deinitCollider] is deprecated, you can remove it from your code!" << std::endl;
}

std::vector<Engine::Entity> Engine::CollisionSystem::checkCollisionsInArea(const DirectX::SimpleMath::Vector2& point, const DirectX::SimpleMath::Vector2& size, const int predicted_collisions)
{
	std::vector<Engine::Entity> collided_entities{};
	collided_entities.reserve(predicted_collisions);

	for (const auto& entity : entities)
	{
		Transform& col_transform = coordinator->getComponent<Engine::Transform>(entity);
		Collider& col_collider   = coordinator->getComponent<Engine::Collider>(entity);

		if (col_transform.position.x < point.x + size.x &&
			col_transform.position.x + col_collider.collider_size.x > point.x &&
			col_transform.position.y < point.y + size.y &&
			col_transform.position.y + col_collider.collider_size.y > size.y)
		{
			collided_entities.push_back(entity);
		}
	}

	return collided_entities;
}

bool Engine::CollisionSystem::checkAABB(const Engine::Entity& a, const Engine::Entity& b)
{
	Transform& a_transform = coordinator->getComponent<Engine::Transform>(a);
	Transform& b_transform = coordinator->getComponent<Engine::Transform>(b);

	Collider& a_collider = coordinator->getComponent<Engine::Collider>(a);
	Collider& b_collider = coordinator->getComponent<Engine::Collider>(b);

	return a_transform.position.x < b_transform.position.x + b_collider.collider_size.x &&
		   a_transform.position.x + a_collider.collider_size.x > b_transform.position.x &&
		   a_transform.position.y < b_transform.position.y + b_collider.collider_size.y &&
		   a_transform.position.y + a_collider.collider_size.y > b_transform.position.y;
}