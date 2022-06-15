#include "pch.h"

////! Components !////
/// Rendering ///
#include "SpriteRenderer.h"
#include "UI.h"
/// Positioning ///
#include "Transform.h"

////! Needed to complile !////
#include <SimpleMath.h>

////! Header !////
#include "SceneMainMenu.h"


SceneMainMenu::SceneMainMenu(const GameScenes& game_scene) : Scene(game_scene) {}

void SceneMainMenu::init()
{
	/// Reset scene data struct 
	setDefaultSceneData();

	/// Instantiate ECS
	ecs = std::make_unique<Engine::Coordinator>();
	ecs->init();

	initComponents();
	initSystems();
	initSounds();
}

void SceneMainMenu::initComponents()
{
	/// Rendering ///
	ecs->registerComponent<Engine::SpriteRenderer>();
	ecs->registerComponent<Engine::TextRenderer>();
	ecs->registerComponent<Engine::UI>();
	ecs->registerComponent<Engine::UIButton>();
	ecs->registerComponent<Engine::UIButtonGroup>();
	ecs->registerComponent<Engine::UIButtonAbility>();
	ecs->registerComponent<Engine::Camera>();

	/// Positioning ///
	ecs->registerComponent<Engine::Transform>();

}

void SceneMainMenu::initSystems()
{
	////! Camera !////
	camera_system = ecs->registerSystem<Engine::CameraSystem>();
	{
		Engine::Signature signature;
		signature.set(ecs->getComponentType<Engine::Transform>());
		signature.set(ecs->getComponentType<Engine::Camera>());
		ecs->setSystemSignature<Engine::CameraSystem>(signature);
	}
	camera_system->init(ecs.get());

	////! Camera Entity !////
	/// Get current window information
	DirectX::SimpleMath::Vector2 window_size = Engine::Driver::getDriver()->getWindowSize();
	const float aspect_ratio = window_size.x / window_size.y;
	/// Create main camera component
	main_camera = ecs->createEntity();
	ecs->addComponent<Engine::Transform>(main_camera,
		{
			DirectX::SimpleMath::Vector3(0.0f, 200.0f, 200.0f),
			DirectX::SimpleMath::Vector2::One, 0 
		});
	ecs->addComponent<Engine::Camera>(main_camera, {}).makeProjectionTransform(0.25f * XM_PI, aspect_ratio, 1.0f, 10000.0f,
			DirectX::SimpleMath::Vector3::UnitY, DirectX::SimpleMath::Vector3::Zero);
	/// Assign camera component to DirectX camera
	Engine::Driver::getDriver()->createDrawData(
		&ecs->getComponent<Engine::Camera>(main_camera), nullptr);

	////! Sprite Render System !////
	render_system = ecs->registerSystem<Engine::RenderSystem>();
	{
		Engine::Signature signature;
		signature.set(ecs->getComponentType<Engine::Transform>());
		signature.set(ecs->getComponentType<Engine::SpriteRenderer>());
		ecs->setSystemSignature<Engine::RenderSystem>(signature);
	}
	render_system->init(ecs.get(), main_camera);

	////! Text Render System !////
	text_system = ecs->registerSystem<Engine::TextSystem>();
	{
		Engine::Signature signature;
		signature.set(ecs->getComponentType<Engine::Transform>());
		signature.set(ecs->getComponentType<Engine::TextRenderer>());
		ecs->setSystemSignature<Engine::TextSystem>(signature);
	}
	text_system->init(ecs.get());

	////! Scaling Render System !////
	scale_system = ecs->registerSystem<Engine::AutoScaleSystem>();
	{
		Engine::Signature signature;
		signature.set(ecs->getComponentType<Engine::Transform>());
		ecs->setSystemSignature<Engine::AutoScaleSystem>(signature);
	}
	scale_system->init(ecs.get());

	////! UI Manager !////
	ui_canvas = ecs->registerSystem<Engine::UiManager>();
	{
		Engine::Signature signature;
		signature.set(ecs->getComponentType<Engine::Transform>());
		signature.set(ecs->getComponentType<Engine::UI>());
		ecs->setSystemSignature<Engine::UiManager>(signature);
	}
	ui_canvas->init(ecs.get(), render_system.get(), text_system.get(), getCurrentScene());

}

void SceneMainMenu::initSounds()
{
	// Button press 
	button_press_sound.sound_name = Engine::Driver::getLoadHandler()->getResourcePath(Engine::LoadHandler::Resources::DEBUG_AUDIO);
}

void SceneMainMenu::update(DX::StepTimer const& timer)
{
	camera_system->update(timer);
	ui_canvas->update(timer);
	ui_canvas->moveCursor(x_input * booster, y_input * booster, timer);

	/// Update the ECS
	ecs->update();
}

void SceneMainMenu::render(DX::StepTimer const& timer)
{
	scale_system->update(timer);
	render_system->update(timer);
	text_system->update();
}

void SceneMainMenu::onKey(const DirectX::Keyboard::KeyboardStateTracker& keyboard_data)
{
	// Quit the game upon hitting escape
	if (keyboard_data.pressed.Escape || keyboard_data.pressed.C)
	{
		PostQuitMessage(0);
	}
	/// Arcade machine input ///
	// Clicking
	if (keyboard_data.pressed.A)
	{
		inputLeftClick();
	}
	if (keyboard_data.pressed.X)
	{
		inputRightClick();
	}

	// Movement
	if (keyboard_data.pressed.LeftShift)
	{
		booster = 2.5F;
	}
	else if (keyboard_data.released.LeftShift)
	{
		booster = 1.0F;
	}

	if (keyboard_data.pressed.Left && x_input == 0.0F)
	{
		x_input = -1.0F;
	}
	else if (keyboard_data.released.Left && x_input <= 0.0F)
	{
		x_input = 0.0F;
	}

	if (keyboard_data.pressed.Right && x_input == 0.0F)
	{
		x_input = 1.0F;
	}
	else if (keyboard_data.released.Right && x_input >= 0.0F)
	{
		x_input = 0.0F;
	}

	if (keyboard_data.pressed.Up && y_input == 0.0F)
	{
		y_input = -1.0F;
	}
	else if (keyboard_data.released.Up && y_input <= 0.0F)
	{
		y_input = 0.0F;
	}

	if (keyboard_data.pressed.Down && y_input == 0.0F)
	{
		y_input = 1.0F;
	}
	else if (keyboard_data.released.Down && y_input >= 0.0F)
	{
		y_input = 0.0F;
	}
}

void SceneMainMenu::onMouse(const DirectX::Mouse::ButtonStateTracker& mouse_data)
{
	if (mouse_data.leftButton == mouse_data.PRESSED)
	{
		inputLeftClick();
	}
	if (mouse_data.rightButton == mouse_data.PRESSED)
	{
		inputRightClick();
	}
}

void SceneMainMenu::inputLeftClick()
{
	Vector2 cur_pos2d = Vector2(ui_canvas->getCursorCoordinates());
    Engine::Driver::getAudioHandler()->playSound(button_press_sound);
	ui_canvas->checkCollision(Vector3{ cur_pos2d.x, cur_pos2d.y, 0.0f });
}

void SceneMainMenu::inputRightClick() {}