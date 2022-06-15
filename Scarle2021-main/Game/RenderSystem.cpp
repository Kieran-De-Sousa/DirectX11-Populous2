// Created by Ben on 28/02/2022

#include "pch.h"

#include <iostream>

#include "helper.h"
#include "DDSTextureLoader.h"

#include "Transform.h"
#include "SpriteRenderer.h"
#include "Driver.h"

#include "RenderSystem.h"

Engine::RenderSystem::RenderSystem() : 
	coordinator(nullptr), draw_data(nullptr), common_states(nullptr), d3d_device(nullptr)
{
	// Can't initialise variables in here because of header only circular dependency issues
}

void Engine::RenderSystem::init(Coordinator* _coordinator, const Engine::Entity& _main_camera)
{
	coordinator = _coordinator;
	draw_data = Engine::Driver::getDriver()->draw_data_2D; // Now ECS and Driver are linked... but we have to share
	common_states = Engine::Driver::getDriver()->states;   // data between them somehow..?
	d3d_device = Engine::Driver::getDriver()->d3d_device.Get();

	main_camera = _main_camera;
}

void Engine::RenderSystem::update(DX::StepTimer const& timer)
{
	/* Don't try to render anything before the first update
	if (timer.GetFrameCount() == 0)
	{
		// Last moment branch - not good!! 
		// This might become 'not a problem' later but for now its needed
		// Ok well its not actually needed because its handled in Driver->Render()
		// But if you change something and the game crashes instantly then it might be this!
		return;
	}
	*/

	draw_data->m_Sprites->Begin(SpriteSortMode_Deferred, common_states->NonPremultiplied());
	for (const Entity& entity : entities)
	{
		Transform& transform = coordinator->getComponent<Transform>(entity);
		SpriteRenderer& sprite_renderer = coordinator->getComponent<SpriteRenderer>(entity);

		updateSpriteAnimator(entity, timer.GetElapsedSeconds());
		draw_data->m_Sprites->Draw(sprite_renderer.texture, transform.position, sprite_renderer.source_rect, 
			                       sprite_renderer.colour, transform.rotation, sprite_renderer.origin, 
			                       (transform.scale.x, transform.scale.y), sprite_renderer.sprite_effects);
	}
	draw_data->m_Sprites->End();
}

void Engine::RenderSystem::initSprite(const std::string& texture_path, const Entity& entity)
{
	Transform& transform = coordinator->getComponent<Transform>(entity);
	SpriteRenderer& sprite_renderer = coordinator->getComponent<SpriteRenderer>(entity);

	LoadedTexture loaded_texture = loadTexture(texture_path);
	sprite_renderer.texture = loaded_texture.texture;

	// Find out the size of the image
	ID3D11Resource* pResource;
	D3D11_TEXTURE2D_DESC Desc;
	sprite_renderer.texture->GetResource(&pResource);
	((ID3D11Texture2D*)pResource)->GetDesc(&Desc);

	// Decouples texture dimensions away from render system so other systems can access information
	// E.G: Provides UI manager auto window scaling capabilities 
	sprite_renderer.texture_dimensions = { DirectX::SimpleMath::Vector2((float)Desc.Width, (float)Desc.Height) };

	// Around which rotation and scaling is done
	sprite_renderer.origin = 0.5f * sprite_renderer.texture_dimensions;
	sprite_renderer.colour = DirectX::SimpleMath::Color(1, 1, 1, 1);

	SetRect(sprite_renderer.source_rect, 1, 1, sprite_renderer.texture_dimensions.x, sprite_renderer.texture_dimensions.y);

	if (!loaded_texture.successful)
	{
		transform.scale = { 20.0f, 20.0f};
		sprite_renderer.colour = Colors::HotPink;
	}
}

/// Kieran - REFACTOR ME PLEASE
void Engine::RenderSystem::updateSprite(const std::string& texture_path, const Entity& entity)
{
	SpriteRenderer& sprite_renderer = coordinator->getComponent<SpriteRenderer>(entity);
	LoadedTexture loaded_texture = loadTexture(texture_path);
	sprite_renderer.texture = loaded_texture.texture;

	if (!loaded_texture.successful)
	{
		sprite_renderer.colour = Colors::HotPink;
	}
}

void Engine::RenderSystem::deinitSprite(const Entity& entity)
{
	ID3D11ShaderResourceView* texture = coordinator->getComponent<SpriteRenderer>(entity).texture;
	if (texture)
	{
		texture->Release();
		texture = nullptr;
	}
}

void Engine::RenderSystem::initAnimator(const Entity& entity, const std::unordered_map<std::string, std::vector<std::array<LONG, 4>>>& _animations, const double _tick_speed)
{
	SpriteRenderer& sprite_renderer = coordinator->getComponent<SpriteRenderer>(entity);

	sprite_renderer.animator.in_use = true;
	sprite_renderer.animator.animations = _animations;
	sprite_renderer.animator.tick_speed = _tick_speed;

	sprite_renderer.animator.current_frame = -1;
	sprite_renderer.animator.clock = 0.0;
	sprite_renderer.animator.current_animation = _animations.begin()->first;

	updateSpriteAnimator(entity, 100); // 100 so the first animation just plays instantly
}

void Engine::RenderSystem::changeAnimation(const Entity& entity, const std::string& animation_name)
{
	Animator& animator = coordinator->getComponent<SpriteRenderer>(entity).animator;

	//assert(!animator.in_use && "The sprite on which your trying to change the animation doesn't have an setup animation component! Call initAnimator first.");
	//assert(animator.animations.find(animation_name) == animator.animations.end() && "The animation which your trying to change to doesn't exist!");

	animator.current_animation = animation_name;
}

void Engine::RenderSystem::refreshSpriteZOrder()
{
	std::list<SpriteRenderer*> sprite_renders;

	for (const Entity& entity : entities)
	{
		SpriteRenderer* sprite_renderer = &coordinator->getComponent<SpriteRenderer>(entity);
		sprite_renders.push_back(sprite_renderer);
	}

	sprite_renders.sort([](const SpriteRenderer* lhs, const SpriteRenderer* rhs) { return lhs->z_order < rhs->z_order; });
}

void Engine::RenderSystem::updateSpriteAnimator(const Entity& entity, const double delta_time)
{
	if (!play_sprite_animations) return;

	SpriteRenderer& sprite_renderer = coordinator->getComponent<SpriteRenderer>(entity);
	Animator& animator = sprite_renderer.animator;

	if (animator.in_use)
	{
		animator.clock += delta_time;
		if (animator.clock > animator.tick_speed)
		{
			if (animator.current_frame < animator.animations[animator.current_animation].size() - 1)
			{
				animator.current_frame++;
			}
			else
			{
				sprite_renderer.animator.current_frame = 0;
			}

			std::array<LONG, 4> frame = animator.animations[animator.current_animation][animator.current_frame];

			if (sprite_renderer.source_rect) // make this a smart pointer ;p
			{
				delete sprite_renderer.source_rect;
			}

			RECT* rect = new RECT();
			rect->left = frame[0];
			rect->top = frame[1];
			rect->right = frame[2];
			rect->bottom = frame[3];
			sprite_renderer.source_rect = rect;

			animator.clock = 0;
		}
	}
}

Engine::RenderSystem::LoadedTexture Engine::RenderSystem::loadTexture(const std::string& texture_path)
{
	string fullfilename = "../Assets/" + texture_path + ".dds";
	if (textures.find(fullfilename) != textures.end())
	{
		return {textures[fullfilename], true };
	}

	ID3D11ShaderResourceView* new_texture;
	HRESULT hr = CreateDDSTextureFromFile(d3d_device, Helper::charToWChar(fullfilename.c_str()), nullptr, &new_texture);
	if (hr != S_OK)
	{
		CreateDDSTextureFromFile(d3d_device, L"../Assets/white.dds", nullptr, &new_texture);

		// Current issue: This will only be called once
		// However, missing textures will always be fixed so shouldn't be a problem..? Hopefully?
		cout << "Missing Texture : " << texture_path << endl;
	}

	textures[fullfilename] = new_texture;
	return { new_texture, hr == S_OK };
}

bool Engine::RenderSystem::getPlaySpriteAnimations() const
{
	return play_sprite_animations;
}

void Engine::RenderSystem::setPlaySpriteAnimations(bool _play_sprite_animations)
{
	play_sprite_animations = _play_sprite_animations;
}
