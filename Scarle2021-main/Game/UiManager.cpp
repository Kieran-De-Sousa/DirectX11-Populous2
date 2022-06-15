#include "pch.h"
#include "UiManager.h"

#include "Driver.h"

#pragma region Instantiation of UI
/// Only here due to test scenes calling this init method
void Engine::UiManager::init(Coordinator* _coordinator, Engine::RenderSystem* _renderSystem, Engine::TextSystem* _textSystem, const GameScenes& scene_type)
{
	coordinator = _coordinator;
	render_system = _renderSystem;
	text_system = _textSystem;
	gameScene = scene_type;


	// Used for autoscaling
	windowDimensions = Engine::Driver::getDriver()->getWindowSize();

	createUI(gameScene);
}

void Engine::UiManager::init(Coordinator* _coordinator, Engine::RenderSystem* _renderSystem, Engine::TextSystem* _textSystem, const GameScenes& scene_type,  AbilityManager* _ability_manager)
{
	coordinator = _coordinator;
	render_system = _renderSystem;
	text_system = _textSystem;
	gameScene = scene_type;
	ability_manager = _ability_manager;


	// Used for relative placement of entities
	windowDimensions = Engine::Driver::getDriver()->getWindowSize();

	createUI(gameScene);
}

void Engine::UiManager::createUI(const GameScenes& scene_type)
{
	/// Create approprite scene based on scene passed in instantiation
	switch (scene_type)
	{
		case GameScenes::MAIN_MENU:
		{
			createMainMenuScene();
			break;
		}
		case GameScenes::GAME:
		{
			createGameScene();
			break;
		}
		default:
		{
			break;
		}
	}

	// Create UI cursor entity last (Solves additional Z-ordering issues)
	cursor = coordinator->createEntity();
	coordinator->addComponent<Engine::Transform>(cursor,
		{
			DirectX::SimpleMath::Vector3(windowDimensions.x / 2, windowDimensions.y / 2, 0.0F),
			DirectX::SimpleMath::Vector2::One / 2,
			0.0f
		});
	coordinator->addComponent<Engine::SpriteRenderer>(cursor, {});
	coordinator->addComponent<Engine::UI>(cursor, {});
	render_system->initSprite("Cursor-1", cursor);

	SpriteRenderer& sprite = coordinator->getComponent<SpriteRenderer>(cursor);
	sprite.origin = { DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f) };
	sprite.z_order = 10;
	uiElements.push_back(cursor);
}

void Engine::UiManager::createMainMenuScene()
{
	auto background = coordinator->createEntity();
	/// Component Adding
	coordinator->addComponent<Engine::Transform>(background,
		{
			DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f),
			DirectX::SimpleMath::Vector2::One,
			0.0f
		});
	coordinator->addComponent<Engine::SpriteRenderer>(background, {});
	coordinator->addComponent<Engine::UI>(background,
		{
			UI::UIType::IMAGE
		});
	render_system->initSprite("PopulousII MainMenu.png", background);

	/// Component Setting
	SpriteRenderer& sprite = coordinator->getComponent<SpriteRenderer>(background);
	Transform& transform = coordinator->getComponent<Transform>(background);
	transform.position =
	{
		windowDimensions.x / 2,
		windowDimensions.y / 2,
		0.0f
	};
	/// Magic numbers for relative scale of 800x600, autoscale system adjusts appropriately
	transform.scale = { 1.2f, 1.45f };

	uiElements.push_back(background);

	createMenuButtons();
}

void Engine::UiManager::createMenuButtons()
{
	for (int i = 0; i < MENU_BUTTONS; ++i)
	{
		auto button = coordinator->createEntity();
		/// Component Adding
		coordinator->addComponent<Engine::Transform>(button,
			{
				DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f),
				DirectX::SimpleMath::Vector2::One,
				0.0f
			});
		coordinator->addComponent<Engine::SpriteRenderer>(button, {});
		coordinator->addComponent<Engine::UI>(button,
			{
				UI::UIType::BUTTON
			});
		coordinator->addComponent<Engine::UIButton>(button,{});
		render_system->initSprite("PopulousII MainMenuButton.png", button);

		/// Component Setting
		SpriteRenderer& sprite = coordinator->getComponent<SpriteRenderer>(button);
		Transform& transform = coordinator->getComponent<Transform>(button);
		transform.position =
		{
			windowDimensions.x / 2,
			windowDimensions.y / 2 + sprite.texture_dimensions.y * i + i * 50,
			0.0f
		};
		UIButton& button_type = coordinator->getComponent<UIButton>(button);
		button_type.button_type = UIButton::ButtonType::GROUP;
		button_type.button_role = static_cast<UIButton::ButtonRole>(i + 1);

		// Create menu text to display on top of button
		createMenuText(i + 1, transform, sprite, button_type);

		/// Push button into UI vector
		uiElements.push_back(button);
	}
}

void Engine::UiManager::createMenuText(const int& iterator, const Transform& buttonBasePosition, const SpriteRenderer& buttonSprite, const UIButton& buttonRole)
{
	auto text = coordinator->createEntity();
	coordinator->addComponent<Engine::Transform>(text,
		{
			DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f),
			DirectX::SimpleMath::Vector2::One,
			0.0f
		});
	coordinator->addComponent<Engine::TextRenderer>(text, {});
	coordinator->addComponent<Engine::UI>(text, { UI::UIType::TEXT });
	text_system->initText(buttonRole.button_text.at(static_cast<UIButton::ButtonRole>(iterator)), text);

	Transform& text_transform = coordinator->getComponent<Transform>(text);
	text_transform.position =
	{
		buttonBasePosition.position.x - (buttonSprite.texture_dimensions.x / 2.75f),
		buttonBasePosition.position.y - (buttonSprite.texture_dimensions.y / 2.75f),
		0.0f
	};

	uiElements.push_back(text);
}

void Engine::UiManager::createGameScene()
{
	createAbilityGroupButtons();
	createAbilityButtons();

	manaText = coordinator->createEntity();
	coordinator->addComponent<Engine::Transform>(manaText,
		{
			DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f),
			DirectX::SimpleMath::Vector2::One,
			0.0f
		});
	coordinator->addComponent<Engine::TextRenderer>(manaText, {});
	coordinator->addComponent<Engine::UI>(manaText, { UI::UIType::TEXT });
	text_system->initText("0.0f", manaText);

	Transform& text_transform = coordinator->getComponent<Transform>(manaText);
	text_transform.position =
	{
		windowDimensions.x / 12,
		windowDimensions.y / 12,
		0.0f
	};
	// Half the size of the text
	text_transform.scale = { 0.5f, 0.5f };
}

void Engine::UiManager::createAbilityGroupButtons()
{
	/// Ability Group instantiation
	for (int i = 0; i < ABILITY_GROUPS ; ++i)
	{
		auto groupButton = coordinator->createEntity();

		/// Component Adding
		coordinator->addComponent<Engine::Transform>(groupButton,
			{
				DirectX::SimpleMath::Vector3(1.0f, 1.0f, 0.0f),
				DirectX::SimpleMath::Vector2::One,
				0.0f
			});
		coordinator->addComponent<Engine::SpriteRenderer>(groupButton, {});
		coordinator->addComponent<Engine::UI>(groupButton,
			{
				UI::UIType::BUTTON
			});
		coordinator->addComponent<Engine::UIButton>(groupButton, {});
		coordinator->addComponent<Engine::UIButtonGroup>(groupButton, {});
		render_system->initSprite("ButtonBase", groupButton);

		/// Component Setting
		UIButton& abilityButton = coordinator->getComponent<UIButton>(groupButton);
		abilityButton.button_type = UIButton::ButtonType::GROUP;

		UIButtonGroup& group = coordinator->getComponent<UIButtonGroup>(groupButton);
		group.buttonGroup = static_cast<AbilityCategories>(i);

		// Change sprite of first element to show that button is "active"
		if (i == 0)
		{
			group.selected = true;
			render_system->updateSprite(group.button_textures.at(group.selected), groupButton);
			previousGroup = groupButton;
		}
		SpriteRenderer& sprite = coordinator->getComponent<SpriteRenderer>(groupButton);
		Transform& transform = coordinator->getComponent<Transform>(groupButton);
		transform.position =
		{
			sprite.texture_dimensions.x * (i + 1) ,
			windowDimensions.y / 1.5f + sprite.texture_dimensions.y * i,
			0.0f
		};

		/// Push entity into UI vector
		uiElements.push_back(groupButton);

		/// Create icons for button group
		createAbilityGroupButtonIcons(i, transform, group);
	}
}

void Engine::UiManager::createAbilityGroupButtonIcons(const int& iterator, const Transform& buttonBasePosition, const UIButtonGroup& buttonGroup)
{
	auto icon = coordinator->createEntity();

	/// Component Adding
	coordinator->addComponent<Engine::Transform>(icon,
		{
			DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f),
			DirectX::SimpleMath::Vector2::One,
			0.0f
		});
	coordinator->addComponent<Engine::SpriteRenderer>(icon, {});
	coordinator->addComponent<Engine::UI>(icon,
		{
			UI::UIType::IMAGE
		});	
	render_system->initSprite(buttonGroup.icon_textures.at(static_cast<AbilityCategories>(iterator)), icon);

	/// Component Setting
	SpriteRenderer& icon_sprite = coordinator->getComponent<SpriteRenderer>(icon);
	Transform& icon_transform = coordinator->getComponent<Transform>(icon);
	icon_transform.position =
	{
		buttonBasePosition.position.x,
		buttonBasePosition.position.y,
		0.0f
	};

	uiElements.push_back(icon);
}

void Engine::UiManager::createAbilityButtons()
{
	/// Ability instantiation
	for (int i = 0; i < ABILITIES; ++i)
	{
		auto ability = coordinator->createEntity();

		/// Component Adding
		coordinator->addComponent<Engine::Transform>(ability,
			{
				DirectX::SimpleMath::Vector3(1.0f, 1.0f, 1.0f),
				DirectX::SimpleMath::Vector2::One,
				0.0f
			});
		coordinator->addComponent<Engine::SpriteRenderer>(ability, {});
		coordinator->addComponent<Engine::UI>(ability,
			{
				UI::UIType::BUTTON
			});
		coordinator->addComponent<Engine::UIButton>(ability, {});
		coordinator->addComponent<Engine::UIButtonAbility>(ability, {});
		render_system->initSprite("ButtonBase", ability);

		/// Component Setting
		SpriteRenderer& sprite = coordinator->getComponent<SpriteRenderer>(ability);
		Transform& transform = coordinator->getComponent<Transform>(ability);
		transform.position =
		{
			windowDimensions.x - (sprite.texture_dimensions.x * i) - sprite.texture_dimensions.x,
			windowDimensions.y / 1.5f + (sprite.texture_dimensions.y * i),
			0.0f
		};

		UIButton& abilityButton = coordinator->getComponent<UIButton>(ability);
		abilityButton.button_type = UIButton::ButtonType::ABILITY;

		UIButtonAbility& abilityPower = coordinator->getComponent<UIButtonAbility>(ability);
		abilityPower.abilityLevel = i;

		// Change sprite of first element to show that button is "active"
		if (i == 0)
		{
			abilityPower.selected = true;
			render_system->updateSprite(abilityPower.button_textures.at(abilityPower.selected), ability);
			previousAbility = ability;
		}

		/// Push entity into UI vector
		uiElements.push_back(ability);
	}
}

#pragma endregion

#pragma region Update the UI
void Engine::UiManager::update(const DX::StepTimer& timer)
{
	updateCursorPosition(timer);
	switch (gameScene)
	{
	// Update selected sprites if we are in a game scene
	case GameScenes::GAME:
		for (const Entity& entity : uiElements)
		{
			UI& ui = coordinator->getComponent<UI>(entity);
			// Check for collision only on button elements
			if (ui.ui_type == UI::UIType::BUTTON)
			{
				SpriteRenderer& sprite = coordinator->getComponent<SpriteRenderer>(entity);
				UIButton& button = coordinator->getComponent<UIButton>(entity);
				if (button.button_type == UIButton::ButtonType::ABILITY)
				{
					UIButtonAbility& ability = coordinator->getComponent<UIButtonAbility>(entity);
					render_system->updateSprite(ability.button_textures.at(ability.selected), entity);
				}
				else if (button.button_type == UIButton::ButtonType::GROUP)
				{
					UIButtonGroup& group = coordinator->getComponent<UIButtonGroup>(entity);
					render_system->updateSprite(group.button_textures.at(group.selected), entity);
				}
			}
		}

		break;

	default:
		break;
	}
}

void Engine::UiManager::updateManaPool(const float _mana_pool)
{
	TextRenderer& text = coordinator->getComponent<TextRenderer>(manaText);
	text.text = "Mana: " + std::to_string(_mana_pool);
}

void Engine::UiManager::updateCursorPosition(const DX::StepTimer& timer)
{
	Transform& transform = coordinator->getComponent<Transform>(cursor);
	transform.position += DirectX::SimpleMath::Vector3(static_cast<float>(Engine::Driver::getDriver()->getGameData()->m_MS.x),
		static_cast<float>(Engine::Driver::getDriver()->getGameData()->m_MS.y),
		0.0f);
}

void Engine::UiManager::moveCursor(float _x, float _y, const DX::StepTimer& timer)
{
	static const float CURSOR_SPEED = 150.0F;
	Transform& transform = coordinator->getComponent<Transform>(cursor);
	transform.position += DirectX::SimpleMath::Vector3(_x * CURSOR_SPEED * timer.GetElapsedSeconds(),
		_y * CURSOR_SPEED * timer.GetElapsedSeconds(),
		0.0f);
}
#pragma endregion

bool Engine::UiManager::checkCollision(DirectX::SimpleMath::Vector3 const& cursorCoords)
{
	// Loop through all UI entities to assess collisions
	for (const Entity& entity : uiElements)
	{
		UI& ui = coordinator->getComponent<UI>(entity);
		// Check for collision only on button elements
		if (ui.ui_type == UI::UIType::BUTTON)
		{
			//UI& ui = coordinator->getComponent<UI>(entity);
			// Get access to relevant components for button entities
			Transform& transform = coordinator->getComponent<Transform>(entity);
			SpriteRenderer& sprite = coordinator->getComponent<SpriteRenderer>(entity);

			if (cursorCoords.x >= transform.position.x - (sprite.texture_dimensions.x / 2 * transform.scale.x) &&
				cursorCoords.x <= transform.position.x + (sprite.texture_dimensions.x / 2 * transform.scale.x) &&
				cursorCoords.y >= transform.position.y - (sprite.texture_dimensions.y / 2 * transform.scale.y) &&
				cursorCoords.y <= transform.position.y + (sprite.texture_dimensions.y / 2 * transform.scale.y))
			{
				switch (gameScene)
				{
					case GameScenes::MAIN_MENU:
					{
						clickMenu(entity);
						break;
					}
					case GameScenes::GAME:
					{
						clickGame(entity);
						break;
					}
					default:
					{
						break;
					}
				}

				// Return value given to the scene after executing required functions
				return true;
			}
		}
	}

	return false;
}

void Engine::UiManager::clickMenu(const Entity& entity)
{
	UIButton& button = coordinator->getComponent<UIButton>(entity);
	switch (button.button_role)
	{
		case UIButton::ButtonRole::PLAY:
		{
			// Hacky way of transitioning game scenes through pointers
			Engine::Driver::getDriver()->getSceneHandler()->getCurrentScene()->setNewScene(GameScenes::GAME);
			break;
		}
		case UIButton::ButtonRole::QUIT:
		{
			// Hacky way of closing the game by calling directX exit message
			Engine::Driver::getDriver()->onExit();
			break;
		}
		default:
		{
			break;
		}
	}
}

void Engine::UiManager::clickGame(const Entity& entity)
{
	UIButton& button = coordinator->getComponent<UIButton>(entity);
	switch (button.button_type)
	{
		case UIButton::ButtonType::ABILITY:
		{
			UIButtonAbility& ability = coordinator->getComponent<UIButtonAbility>(entity);
			ability_manager->SelectLevel(ability.abilityLevel+1);

			// Updates sprites to show new selection
			UIButtonAbility& prev_ability = coordinator->getComponent<UIButtonAbility>(previousAbility);
			prev_ability.selected = false;
			ability.selected = true;
			previousAbility = entity;
			break;
		}
		case UIButton::ButtonType::GROUP:
		{
			UIButtonGroup& group = coordinator->getComponent<UIButtonGroup>(entity);
			ability_manager->SelectAbility(group.buttonGroup);

			// Updates sprites to show new selection
			UIButtonGroup& prev_group = coordinator->getComponent<UIButtonGroup>(previousGroup);
			prev_group.selected = false;
			group.selected = true;
			previousGroup = entity;
			break;
		}
		case UIButton::ButtonType::PAUSE:
		{
			// Pause stuff here
			break;
		}
		default:
			break;
	}
}

DirectX::SimpleMath::Vector3 Engine::UiManager::getCursorCoordinates() const
{
	Transform& transform = coordinator->getComponent<Transform>(cursor);
	return DirectX::SimpleMath::Vector3{ transform.position.x, transform.position.y, 0 };
}

#pragma region LEGACY SYSTEMS
/*LEGACY SYSTEMS
void Engine::UiManager::createUiImage(const string& _fileName)
{
	auto ui_image = new UiImage(_fileName, m_d3dDevice);
	uiElements.push_back(ui_image);
	// uiImages.push_back(ui_image);
}

void Engine::UiManager::createUiText(const string& _text)
{
	auto ui_text = new UiText(_text);
	uiElements.push_back(ui_text);
	// uiTextBoxes.push_back(ui_text);
}

void Engine::UiManager::createUiButton(const string& _filename)
{
	auto ui_button = new UiButton(_filename, m_d3dDevice);
	uiElements.push_back(ui_button);
}

void Engine::UiManager::createCursor(const string& _fileName)
{
	Cursor* cursor = new Cursor(_fileName, m_d3dDevice);
	uiElements.push_back(cursor);
}

void Engine::UiManager::Tick(GameData* _GD)
{
	tracker.Update(_GD->m_MS);
	// Collision detection function only called if left mouse button was clicked
	if (tracker.leftButton == Mouse::ButtonStateTracker::PRESSED)
	{
		// TODO: We are correctly getting the games mouse state, and the updating of images works correctly...
		// TODO: We now need to get the positions of each of the four corners of the image (using getscale and
		// TODO: getimagedimensions), find their location on the viewport (using getpos which is the origin position),
		// TODO: and finally compare the mouse click position to see if it is within those bounds
		// createUiText("I WORK!");
		// for (auto& ui : uiImages)
		// {
		// 	ui->setImage("ButtonBaseSelected", m_d3dDevice);
		// }
		uiCollisionDetection(_GD);
	}

	// Call tick in every derived class of uiElements
	for (auto& ui : uiElements)
	{
		ui->Tick(_GD);
	}
}

void Engine::UiManager::Draw(DrawData2D* _DD)
{
	// Call draw in every derived class of uiElements
	for (auto& ui : uiElements)
	{
		ui->Draw(_DD);
	}
}

void Engine::UiManager::uiCollisionDetection(GameData* _GD)
{
	// Save X and Y position of cursor as temp variables
	//const auto& x = getCursor()->GetPos().x;
	//const auto& y = getCursor()->GetPos().y;

	// Loop through all interactable/button ui objects
	for (auto& button : uiButtons)
	{
		// TODO: CHECK 4 CORNERS OF IMAGE COMPONENT OF BUTTON AND ASSESS WHETHER MOUSE CLICK WAS THERE
	}
}

std::vector<UiElement*> Engine::UiManager::getUIElements()
{
	return uiElements;
}

void Engine::UiManager::setWindowOrigin()
{
	auto& temp = windowDimensions;
	windowOrigin = temp;
}
*/
#pragma endregion