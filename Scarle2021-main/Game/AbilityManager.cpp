/// Created by Norbert Kupeczki

#include "pch.h"
#include "AbilityManager.h"

AbilityManager::AbilityManager()
{
	initAbilityGroups();
	SelectAbility(AbilityCategories::PEOPLE);
	SelectLevel(1);
}

void AbilityManager::SelectAbility(AbilityCategories new_group)
{
	ability_category_selected = new_group;
	std::cout << ">> " + ability_groups[static_cast<unsigned int>(ability_category_selected)]->getCategoryName() + " ability category selecred <<\n";
}

void AbilityManager::SelectLevel(unsigned int new_level)
{
	ability_level = new_level - 1;
	std::cout << ">> " + ability_groups[static_cast<unsigned int>(ability_category_selected)]->getAbilityName(ability_level) + " ability selected <<\n";
}

bool AbilityManager::UseAbility(Vector2 cursor_pos)
{
	return ability_groups[static_cast<unsigned int>(ability_category_selected)]->useAbility(ability_level, cursor_pos);
}

bool  AbilityManager::AIUseAbility(Vector2 spawn_point, AbilityCategories ability_category, unsigned int ai_ability_lvl)
{
	return ability_groups[static_cast<unsigned int>(ability_category)]->useAbility(ai_ability_lvl, spawn_point);
}

bool AbilityManager::IsAffectingMap()
{
	return (ability_category_selected == AbilityCategories::PEOPLE && ability_level == 0);
}

bool AbilityManager::IsSummoningHero()
{
	return ability_groups[static_cast<unsigned int>(ability_category_selected)]->IsSummoningHero(ability_level);
}

bool AbilityManager::IsAISummoningHero(AbilityCategories ai_category, unsigned int ai_ability_lvl)
{
	return ability_groups[static_cast<unsigned int>(ai_category)]->IsSummoningHero(ai_ability_lvl);
}

Engine::Emitter AbilityManager::getEmitterData()
{
	return ability_groups[static_cast<unsigned int>(ability_category_selected)]->getEmitterData(ability_level);
}

Engine::Emitter AbilityManager::AIgetEmitterData(AbilityCategories ai_category, unsigned int ai_ability_lvl)
{
	return ability_groups[static_cast<unsigned int>(ai_category)]->getEmitterData(ai_ability_lvl);
}

void AbilityManager::initAbilityGroups()
{
	for (auto& category : ability_categories)
	{
		ability_groups.emplace_back(std::make_unique<AbilityGroup>(category));
	}
}
