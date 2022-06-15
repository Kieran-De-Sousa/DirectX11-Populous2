#pragma once
#include "pch.h"

// Engine
#include "System.h"
#include "Coordinator.h"
// Rendering systems
#include "RenderSystem.h"
#include "TextSystem.h"
// Ability System linking
#include "AbilityManager.h"
// Components
#include "SpriteRenderer.h"
#include "Transform.h"
#include "UI.h"
// Scenes
#include "SceneData.h"
// STL/Helpers
#include <vector>
#include <iostream>

/* LEGACY SYSTEMS
#include "UiElement.h"
#include "UiImage.h"
#include "UiText.h"
#include "UiButton.h"
#include "Cursor.h"
*/


namespace Engine
{
	class UiManager : public Engine::System
	{
	public:
		UiManager() = default;
		~UiManager() = default;

		/// <summary>
		/// Used only for test scenes to compile game
		/// </summary>
		void init(Coordinator* _coordinator, Engine::RenderSystem* _renderSystem, Engine::TextSystem* _textSystem, const GameScenes& scene_type);
		/// <summary>
		/// Creates link to ECS coordinator and rendering systems for render creation. Recieves scene type to create appropriate UI appearance for scene
		/// and creates a link to ability manager (used for the game scene)
		/// </summary>
		/// <param name="_coordinator">| Pointer to the scene's coordinator</param>
		/// <param name="_renderSystem">| Pointer to the scene's render system</param>
		/// <param name="_textSystem">| Pointer to the scene's text system</param>
		/// <param name="scene_type">| Enum type of the scene</param>
		/// <param name="_ability_manager">| Pointer to the scene's ability system</param>
		void init(Coordinator* _coordinator, Engine::RenderSystem* _renderSystem, Engine::TextSystem* _textSystem, const GameScenes& scene_type, AbilityManager* _ability_manager);

		/// <summary>
		/// Updates game cursor location
		/// </summary>
		/// <param name="timer">| Delta time</param>
		void update(const DX::StepTimer& timer);

		/// <summary>
		/// Recieves mana pool every update and updates text for it accordingly
		/// </summary>
		/// <param name="_mana_pool">| Mana pool float value</param>
		void updateManaPool(const float _mana_pool);

		//--------------------------------------------! Click Detection !---------------------------------------------//
		/// <summary>
		/// Called whenever an "onClick" event is registered. Returns true if clicking on UI button. Loops through 'UIElements' vector, 
		/// checks if the 'UIType' of the element is a button, checks click location if it's within the sprite bounds and calls 
		/// game state function to resolve result.
		/// </summary>
		/// <param name="cursorCoords">| Vector3 position of in-game cursor</param>
		/// <returns>Boolean value if any UI elements where clicked</returns>
		bool checkCollision(DirectX::SimpleMath::Vector3 const& cursorCoords);

		//----------------------------------------------------------------------------------------------------------//


		//-----------------------------------------------! Cursor !------------------------------------------------//
		/// <summary>
		/// Updates sprite location of the cursor entity based on mouse position
		/// </summary>
		/// <param name="timer">| Delta time</param>
		void updateCursorPosition(const DX::StepTimer& timer);

		/// <summary>
		/// Moves cursor for arcade joystick controls. Should be called every update
		/// </summary>
		/// <param name="_x">| X movement from joystick during the tick</param>
		/// <param name="_y">| Y movement from joystick during the tick</param>
		/// <param name="timer">| Delta time</param>
		void moveCursor(float _x, float _y, const DX::StepTimer& timer);

		/// <summary>
		/// Return Vector3 coordinates of cursor origin position
		/// </summary>
		/// <returns>Vector3 Cursor Origin</returns>
		DirectX::SimpleMath::Vector3 getCursorCoordinates() const;

		//----------------------------------------------------------------------------------------------------------//

	private:
		/// <summary>
		/// Initialise UI elements in default positions depending on game scene
		/// </summary>
		void createUI(const GameScenes& scene_type = GameScenes::UNDEFINED);
		//-----------------------------------! Main Menu Instantiation !-------------------------------------//
		void createMainMenuScene();
		void createMenuButtons();
		/// <summary>
		/// Creates text to place on the button locations to display their purpose
		/// </summary>
		/// <param name="iterator">| For loop iterator from 'createAbilityGroupButtons'</param>
		/// <param name="buttonBasePosition">| Position of button</param>
		/// <param name="buttonSprite">| Sprite component of button</param>
		/// <param name="buttonRole">| Action button should do</param>
		void createMenuText(const int& iterator, const Transform& buttonBasePosition, const SpriteRenderer& buttonSprite, const UIButton& buttonRole);

		//--------------------------------------------------------------------------------------------------//


		//-----------------------------------! Game Scene Instantiation !-----------------------------------//
		void createGameScene();
		void createAbilityGroupButtons();
		/// <summary>
		/// Creates icons to place on the button locations to display their power
		/// </summary>
		/// <param name="iterator">| For loop iterator from 'createAbilityGroupButtons'</param>
		/// <param name="buttonBasePosition">| Position of group button</param>
		/// <param name="buttonGroup">| ButtonGroup component of button</param>
		void createAbilityGroupButtonIcons(const int& iterator, const Transform& buttonBasePosition, const UIButtonGroup& buttonGroup);
		void createAbilityButtons();

		//--------------------------------------------------------------------------------------------------//


		//----------------------------------------! Click Handling !----------------------------------------//
		/// <summary>
		/// Click detection handling for main menu
		/// </summary>
		/// <param name="entity">| Entity clicked on</param>
		void clickMenu(const Entity& entity);
		/// <summary>
		/// Click detection handling for in-game
		/// </summary>
		/// <param name="entity">| Entity clicked on</param>
		void clickGame(const Entity& entity);

		//--------------------------------------------------------------------------------------------------//

		//----------------------------------------------! ECS !---------------------------------------------//
		Coordinator* coordinator;
		Engine::RenderSystem* render_system;
		Engine::TextSystem* text_system;
		AbilityManager* ability_manager;

		//--------------------------------------------------------------------------------------------------//

		//------------------------------------------! Game State !------------------------------------------//
		GameScenes gameScene = GameScenes::UNDEFINED;
		DirectX::SimpleMath::Vector2 windowDimensions = {0, 0};
		// Vector
		std::vector<Engine::Entity> uiElements;
		// Cursor
		Engine::Entity cursor;
		// Game Buttons
		Engine::Entity previousAbility;
		Engine::Entity previousGroup;
		// Mana Text
		Engine::Entity manaText;
		//--------------------------------------------------------------------------------------------------//

		//-----------------------------------! Constants !-----------------------------------//
		/// Main Menu
		const int MENU_BUTTONS = 2;
		/// Game
		const int ABILITY_GROUPS = 6;
		const int ABILITIES = 5;
		//-----------------------------------------------------------------------------------//

#pragma region Legacy Systems
		/*LEGACY SYSTEMS FUNCTIONS
		// UI Object Instantiation
		void createUiImage(const string& _fileName);
		void createUiText(const string& _text);
		void createUiButton(const string& _filename);
		void createCursor(const string& _fileName);

		// Called by 'Game.cpp', calls corresponding functions in every UI object
		virtual void Tick(GameData* _GD);
		virtual void Draw(DrawData2D* _DD);

		void setWindowOrigin();
		//brief Return vector of all UI components
		//return UiElements vector attribute (variable)
		std::vector<UiElement*> getUIElements();
		//brief Checks mouse position against UiButton class instances
		//param _GD
		void uiCollisionDetection(GameData* _GD);
		*/
		/* LEGACY SYSTEMS PRIVATE MEMBERS
		/// Game State
		ID3D11Device* m_d3dDevice; // Keep a private copy of the device
		Vector2 windowDimensions;
		Vector2 windowOrigin;

		/// UI (Singular instances)
		std::unique_ptr<Cursor> gameCursor; // Cursor 2D object, used for ability system

		/// UI (Vectors)
		std::vector<UiElement*> uiElements; // Contains all instances of UI components
		// Vectors that can contain specific ui classes (most likely not required)
		std::vector<UiImage*> uiImages;
		std::vector<UiText*> uiTextBoxes;
		std::vector<UiButton*> uiButtons;
		*/
#pragma endregion
	};
}