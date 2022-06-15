// Created by Ben on 28 / 02 / 2022

#include "pch.h"

#include "Cam.h"
#include "Transform.h"
#include "CameraSystem.h"

Engine::CameraSystem::CameraSystem() : coordinator(nullptr)
{
	// Can't initialise coordinator in here because of header only circular dependency issues
}

void Engine::CameraSystem::init(Coordinator* _coordinator)
{
	coordinator = _coordinator;
}

void Engine::CameraSystem::update(const DX::StepTimer& timer)
{
	for (const Entity& entity : entities)
	{
		Transform& transform = coordinator->getComponent<Transform>(entity);
		Camera& camera = coordinator->getComponent<Camera>(entity);
	
		camera.proj_matrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(camera.field_of_view, 
			camera.aspect_ratio, camera.near_plane_distance, camera.far_plane_distance);
		camera.view_matrix = DirectX::SimpleMath::Matrix::CreateLookAt(transform.position, 
			camera.target, camera.up);
	}
}
