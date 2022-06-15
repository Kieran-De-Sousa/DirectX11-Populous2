#include "pch.h"
#include "BackgroundCloudSystem.h"

#include "Transform.h"
#include "SpriteRenderer.h"
#include "Cloud.h"

Engine::BackgroundCloudSystem::BackgroundCloudSystem() : coordinator(nullptr)
{
	// Can't initialise variables in here because of header only circular dependency issues
}

void Engine::BackgroundCloudSystem::init(Coordinator* _coordinator, const DirectX::SimpleMath::Vector2& _screen_dimensions, const float _cloud_move_speed)
{
	coordinator = _coordinator;
	screen_dimensions = _screen_dimensions;
	cloud_move_speed = _cloud_move_speed;
}

void Engine::BackgroundCloudSystem::update(const DX::StepTimer& timer)
{
	for (const Entity& entity : entities)
	{
		Transform& transform = coordinator->getComponent<Transform>(entity);
		SpriteRenderer& sprite_renderer = coordinator->getComponent<SpriteRenderer>(entity);
		float sprite_size = 800; // sprite_renderer.texture_dimensions.x * transform.scale; // ? will this be ok? no. sprite size / 2

		transform.position.x += static_cast<float>(cloud_move_speed * timer.GetElapsedSeconds());

		if (transform.position.x > 1200)
		{
			transform.position.x = -400; //- sprite_size;
		}
	}
}

void Engine::BackgroundCloudSystem::createClouds(const std::shared_ptr<RenderSystem>& render_system, const CloudsCreationData& settings)
{
	float x_pos = 0;
	float y_pos = -200; // Why does this need to start on -200?

	while (true)
	{
		// Create entity
		Engine::Entity cloud		   = coordinator->createEntity();
		
		coordinator->addComponent<Engine::Transform>(cloud,
			{ DirectX::SimpleMath::Vector3(x_pos, y_pos, 0), DirectX::SimpleMath::Vector2(settings.cloud_scale, settings.cloud_scale), 1 });
		

		Engine::SpriteRenderer& sprite = coordinator->addComponent<Engine::SpriteRenderer>(cloud, {});
		sprite.z_order = settings.z_order;
		render_system->initSprite(settings.cloud_path, cloud);
		// render_system->refreshSpriteZOrder();

		// Move position for next entity
		x_pos += settings.x_increment;
		if (x_pos > screen_dimensions.x)
		{
			x_pos = 0;
			y_pos += settings.y_increment;

			// Break out of loop
			if (y_pos > screen_dimensions.y) break;
		}
	}
}

void Engine::BackgroundCloudSystem::createCloud(const std::shared_ptr<RenderSystem>& render_system, const CloudCreationData& settings)
{
	// Loop through all the images in settings and create an entity for them
	for (size_t i = 0; i < settings.cloud_images.size(); ++i)
	{
		// Create an entity and tag is with cloud so it can be distinguished 
		Engine::Entity cloud = coordinator->createEntity();
		coordinator->addComponent<Engine::Cloud>(cloud, {});
		
		// Add transform component
		coordinator->addComponent<Engine::Transform>(cloud,
			{ DirectX::SimpleMath::Vector3(settings.cloud_spacing * i, screen_dimensions.y / 2, 0), {1.5, 1.5}, 0});

		// Add and init sprite component
		Engine::SpriteRenderer& sprite = coordinator->addComponent<Engine::SpriteRenderer>(cloud, {});
		sprite.z_order = settings.z_order;
		render_system->initSprite(settings.cloud_images[i], cloud);	
	}
}
