#include "pch.h"

#include <iostream>

#include "helper.h"

#include "Transform.h"
#include "SpriteRenderer.h"
#include "Driver.h"

#include "TextSystem.h"

Engine::TextSystem::TextSystem() :
	coordinator(nullptr), draw_data(nullptr), common_states(nullptr), d3d_device(nullptr)
{
	// Can't initialise variables in here because of header only circular dependency issues
}

void Engine::TextSystem::init(Coordinator* _coordinator)
{
	coordinator = _coordinator;
	draw_data = Engine::Driver::getDriver()->draw_data_2D; // Links ECS and Driver
	common_states = Engine::Driver::getDriver()->states;
	d3d_device = Engine::Driver::getDriver()->d3d_device.Get();
}

void Engine::TextSystem::update()
{
	// Entities contatining text component are looped through and drawn to screen
	draw_data->m_Sprites->Begin(SpriteSortMode_Deferred, common_states->NonPremultiplied());
	for (const Entity& entity : entities)
	{
		Transform& transform = coordinator->getComponent<Transform>(entity);
		TextRenderer& text_renderer = coordinator->getComponent<TextRenderer>(entity);
		
		text_renderer.font->DrawString(draw_data->m_Sprites.get(), Helper::charToWChar(text_renderer.text.c_str()), 
			transform.position, text_renderer.colour, transform.rotation, text_renderer.origin, (transform.scale.x, transform.scale.y));
	}
	draw_data->m_Sprites->End();
}

void Engine::TextSystem::initText(const std::string& _text, const Entity& entity)
{
	Transform& transform = coordinator->getComponent<Transform>(entity);
	TextRenderer& text_renderer = coordinator->getComponent<TextRenderer>(entity);

	text_renderer.text = _text;

	// Default font supplied through drawdata spritefont struct
	text_renderer.font = draw_data->m_Font.get();

	// TODO: Get size of string on window
	//XMVECTOR& result = text_renderer.font->MeasureString(text_renderer.text);

	//text_renderer.origin = 0.5f * DirectX::SimpleMath::Vector2((float) , (float)...);

	text_renderer.colour = DirectX::SimpleMath::Color(1, 1, 1, 1);
	transform.scale = DirectX::SimpleMath::Vector2::One;
}

// TODO: Provide functionality for creating own sprite

/*SpriteFont* Engine::TextSystem::initFont(const std::string& _filename)
{
	std::unique_ptr<SpriteFont> font;
	font = std::make_unique<SpriteFont>(d3d_device, "../Assets/"
		L"..\\Assets\\italic.spritefont");
	return std::move(font.get());
}
*/