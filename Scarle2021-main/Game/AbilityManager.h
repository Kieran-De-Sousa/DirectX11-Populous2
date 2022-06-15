/// Created by Norbert Kupeczki

#pragma once

#include <vector>
#include "AbilityGroup.h"
#include "AbilityData.h"
#include <string>
#include <SimpleMath.h>
#include "Emitter.h"

using Vector2 = DirectX::SimpleMath::Vector2;

class AbilityManager
{
public:
	AbilityManager();
	~AbilityManager() = default;

	/// <summary>
	/// Sets the ability category.
	/// </summary>
	/// <param name="new_group"></param>
	void SelectAbility(AbilityCategories new_group);
	
	/// <summary>
	/// Sets the ability level.
	/// </summary>
	/// <param name="new_level"></param>
	void SelectLevel(unsigned int new_level);
	
	/// <summary>
	/// Checks whether the selected ability+level combination affects the map.
	/// </summary>
	/// <returns>
	/// Returns true if the selected ability category + ability level combination can affect the map.
	/// </returns>
	bool IsAffectingMap();

	/// <summary>
	/// Checks whether the selected ability+level combination summons a hero.
	/// To be used for the player interactions.
	/// </summary>
	/// <returns>
	/// Returns true if the selected ability category + ability level combination summons a hero.
	/// </returns>
	bool IsSummoningHero();

	/// <summary>
	/// Checks whether the selected ability+level combination summons a hero.
	/// To be used for the AI interactions.
	/// </summary>
	/// <param name="ai_category"></param>
	/// <param name="ai_ability_lvl"></param>
	/// <returns></returns>
	bool IsAISummoningHero(AbilityCategories ai_category, unsigned int ai_ability_lvl);

	/// <summary>
	/// Uses the ability based on the the stored category and level.
	/// To be used for the player interactions.
	/// </summary>
	/// <param name="cursor_pos"></param>
	/// <returns>
	/// Return true if the ability has a particle effect component.
	/// </returns>
	bool UseAbility(Vector2 cursor_pos);

	/// <summary>
	/// Uses the ability based on the the parameters provided.
	/// To be used for the AI interactions.
	/// </summary>
	/// <param name="spawn_point"></param>
	/// <param name="ability_category"></param>
	/// <param name="ai_ability_lvl"></param>
	/// <returns>
	/// Return true if the ability has a particle effect component.
	/// </returns>
	bool AIUseAbility(Vector2 spawn_point, AbilityCategories ai_cat, unsigned int ai_ability_lvl);

	/// <summary>
	/// Mutator function to get the data required to create a particle system
	/// To be used for the player interactions.
	/// </summary>
	/// <returns>
	/// Returns an 'Emitter' type struct that stores the data needed to
	/// create an emitter and a particle system
	/// </returns>
	Engine::Emitter getEmitterData();

	/// <summary>
	/// Mutator function to get the data required to create a particle system
	/// To be used for the AI interactions.
	/// </summary>
	/// <param name="ai_category"></param>
	/// <param name="ai_ability_lvl"></param>
	/// <returns>
	/// Returns an 'Emitter' type struct that stores the data needed to
	/// create an emitter and a particle system
	/// </returns>
	Engine::Emitter AIgetEmitterData(AbilityCategories ai_category, unsigned int ai_ability_lvl);

private:
	std::vector<std::unique_ptr<AbilityGroup>> ability_groups;
	AbilityCategories ability_category_selected = AbilityCategories::UNDEFINED;
	unsigned int ability_level = 0;

	/// <summary>
	/// Creates the ability groups, constructing the system that stores all the ability data
	/// </summary>
	void initAbilityGroups();

	std::vector<AbilityCategories> ability_categories =
	{
		AbilityCategories::PEOPLE,
		AbilityCategories::VEGETATION,
		AbilityCategories::EARTH,
		AbilityCategories::AIR,
		AbilityCategories::WATER,
		AbilityCategories::FIRE
	};
};
