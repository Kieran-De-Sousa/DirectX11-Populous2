// Created by Ben on 22/02/2022
// Resource: https://austinmorlan.com/posts/entity_component_system/ 

#include "pch.h"

#include "Transform.h"
#include "RigidBody.h"

#include "PhysicsSystem.h"

Engine::PhysicsSystem::PhysicsSystem() : coordinator(nullptr)
{
	// Can't initialise coordinator in here because of header only circular dependency issues
}

void Engine::PhysicsSystem::init(Coordinator* _coordinator)
{
	coordinator = _coordinator;
}

void Engine::PhysicsSystem::update(const DX::StepTimer& timer)
{
	const float dt = float(timer.GetElapsedSeconds());

	for (const Entity& entity : entities)
	{
		Transform& transform = coordinator->getComponent<Transform>(entity);
		RigidBody& rigidBody = coordinator->getComponent<RigidBody>(entity);

		transform.position += rigidBody.velocity * dt;

		for (auto& child : coordinator->findChildren(entity))
		{
			Transform& child_transform = coordinator->getComponent<Transform>(child);
			if (child_transform.follow_parent)
			{
				child_transform.position += rigidBody.velocity * dt;
			}
		}

		rigidBody.velocity.x += rigidBody.gravity_scale * dt;
		rigidBody.velocity.y += rigidBody.gravity_scale * dt;
		rigidBody.velocity.z += rigidBody.gravity_scale * dt;
		// rigidBody.velocity += doesn't work. I think this gravity calc is ok??
	}
}
