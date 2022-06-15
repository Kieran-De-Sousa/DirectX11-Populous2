#pragma once

/// Rendering
#include "CameraSystem.h"
#include "RenderSystem.h"
#include "TextSystem.h"
#include "AutoScaleSystem.h"
#include "UiManager.h"

/// ECS
#include "Driver.h"
#include "Coordinator.h"

class SceneMainMenu : public Engine::Scene
{
public:
	SceneMainMenu(const GameScenes& scene_type);
	SceneMainMenu() = default;
	~SceneMainMenu() override = default;

	//-----------------------------------! Virtual Functions Inherrited From Scene !-----------------------------------//
	virtual void init();
	virtual void initComponents();
	virtual void initSystems();
	virtual void initSounds();
	virtual void update(DX::StepTimer const& timer);
	virtual void render(DX::StepTimer const& timer);
	virtual void onKey(const DirectX::Keyboard::KeyboardStateTracker& keyboard_data);
	virtual void onMouse(const DirectX::Mouse::ButtonStateTracker& mouse_data);
	//------------------------------------------------------------------------------------------------------------------//

private:
	void inputLeftClick();
	void inputRightClick();
	////! ECS !////
	std::unique_ptr<Engine::Coordinator> ecs;

	////! Systems !////
	/// Rendering ///
	std::shared_ptr<Engine::CameraSystem> camera_system = nullptr;
	std::shared_ptr<Engine::RenderSystem> render_system = nullptr;
	std::shared_ptr<Engine::TextSystem> text_system = nullptr;
	std::shared_ptr<Engine::AutoScaleSystem> scale_system = nullptr;
	std::shared_ptr<Engine::UiManager> ui_canvas = nullptr;

	////! Game state !////
	/// Entities ///
	Engine::Entity main_camera = Engine::NULL_ENTITY;

	short x_input = 0.0F;
	short y_input = 0.0F;
	unsigned short  booster = 1.0F;
	
	/// <summary>
	/// Button press sound 
	/// </summary>
	Engine::AudioHandler::SoundData button_press_sound{};
};

