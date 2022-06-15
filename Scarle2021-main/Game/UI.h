#pragma once
#include "AbilityData.h"
#include "map"

namespace Engine
{

	/// <note> Inherritance (OOP) conflicts with the design paradigmn of ECS (Entitity Component Systems), therefore
	/// <note> a more effective approach to the one below would be providing all data ANY UI object would require within
	/// <note> the UI component as opposed to creating new UI components that inherit from one another. - Kieran

	//-----------------------------------! UI !-----------------------------------//
	struct UI
	{
		UI() = default;
		~UI() = default;

		enum class UIType : int
		{
			NONE = 0,
			CURSOR = 1,
			TEXT = 2,
			BUTTON = 3,
			IMAGE = 4
		};
		UIType ui_type = UIType::NONE;
	};

	//-----------------------------------! UI Button !-----------------------------------//
	struct UIButton : UI
	{
		UIButton() = default;
		~UIButton() = default;

		UIType ui_type = UIType::BUTTON;

		enum class ButtonType : int
		{
			UNKNOWN = 0,
			GROUP = 1,
			ABILITY = 2,
			MENU = 3,
			PAUSE = 4
		};
		ButtonType button_type = ButtonType::UNKNOWN;

		/// Main Menu
		enum class ButtonRole : int
		{
			UNKNOWN = 0,
			PLAY = 1,
			QUIT = 2
		};
		ButtonRole button_role = ButtonRole::UNKNOWN;

		std::map<ButtonRole, std::string> button_text
		{
			{ButtonRole::PLAY, "PLAY"},
			{ButtonRole::QUIT, "QUIT"}
		};
	};

	//-----------------------------------! UI Button - Ability Groups !-----------------------------------//
	struct UIButtonGroup : UIButton
	{
		UIButtonGroup() = default;
		~UIButtonGroup() = default;

		UIType ui_type = UIType::BUTTON;
		ButtonType button_type = ButtonType::GROUP;
		AbilityCategories buttonGroup = AbilityCategories::UNDEFINED;

		std::map<bool, std::string> button_textures
		{
			{false, "ButtonBase"},
			{true, "ButtonBaseSelected"}
		};
		bool selected = false;

		std::map<AbilityCategories, std::string> icon_textures
		{
			{AbilityCategories::PEOPLE, "ButtonIconPeopleGroup"},
			{AbilityCategories::VEGETATION, "ButtonIconVegetationGroup"},
			{AbilityCategories::EARTH, "ButtonIconEarthGroup"},
			{AbilityCategories::AIR, "ButtonIconAirGroup"},
			{AbilityCategories::WATER, "ButtonIconWaterGroup"},
			{AbilityCategories::FIRE, "ButtonIconFireGroup"}
		};
	};

	//-----------------------------------! UI Button - Abilities !-----------------------------------//
	struct UIButtonAbility : UIButtonGroup
	{
		UIButtonAbility() = default;
		~UIButtonAbility() = default;

		UIType ui_type = UIType::BUTTON;
		ButtonType button_type = ButtonType::ABILITY;
		int abilityLevel = 0;
	};
}