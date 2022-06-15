#include "pch.h"

#include <iostream>

#include "helper.h"
#include "DDSTextureLoader.h"


#include "Transform.h"
#include "SpriteRenderer.h"
#include "Driver.h"

#include "AutoScaleSystem.h"

Engine::AutoScaleSystem::AutoScaleSystem() : 
	coordinator(nullptr), draw_data(nullptr), common_states(nullptr), d3d_device(nullptr)
{
	// Can't initialise variables in here because of header only circular dependency issues - Ben
}

void Engine::AutoScaleSystem::init(Coordinator* _coordinator)
{
	coordinator = _coordinator;
	draw_data = Engine::Driver::getDriver()->draw_data_2D; // Now ECS and Driver are linked... but we have to share
	common_states = Engine::Driver::getDriver()->states;   // data between them somehow..?
	d3d_device = Engine::Driver::getDriver()->d3d_device.Get();

	// Initialise window dimensions from game start
	windowDimensions = Engine::Driver::getDriver()->getDefaultSize();
}

void Engine::AutoScaleSystem::update(DX::StepTimer const& timer)
{
	// Get last frames window dimensions and store in temp variable
	const auto& previousDimensions = windowDimensions;

	// Get current window dimensions
	windowDimensions = Engine::Driver::getDriver()->getWindowSize();

	if (windowDimensions != previousDimensions)
	{

		//// Establish the percentage change
		/// <reference> https://www.geeksforgeeks.org/program-to-find-the-percentage-of-difference-between-two-numbers/
		const float& x_difference = (previousDimensions.x - windowDimensions.x) * 100 / windowDimensions.x;
		const float& y_difference = (previousDimensions.y - windowDimensions.y) * 100 / windowDimensions.y;

		/// Check how to multiply the scale
		int powerX = (windowDimensions.x > previousDimensions.x) ? powerX = 1 : powerX = -1;
		int powerY = (windowDimensions.y > previousDimensions.y) ? powerY = 1 : powerY = -1;
		for (const Entity& entity : entities)
		{
			Transform& transform = coordinator->getComponent<Transform>(entity);

			transform.scale = DirectX::SimpleMath::Vector2(transform.scale.x * x_difference * powerX, 
														   transform.scale.y * y_difference * powerY);
			transform.position = DirectX::SimpleMath::Vector3(transform.position.x * x_difference * powerX,
															  transform.position.y * y_difference * powerY,
															  transform.position.z);
		}
	}
}
