/// Created by Norbert Kupeczki

#include "pch.h"
#include "AbilityGroup.h"
#include "People.h"
#include "Vegetation.h"
#include "Earth.h"
#include "Air.h"
#include "Water.h"
#include "Fire.h"

AbilityGroup::AbilityGroup(AbilityCategories category)
{
	initAbilities(category);
}

std::string AbilityGroup::getCategoryName()
{
	return category_name;
}

std::string AbilityGroup::getAbilityName(unsigned int ability_lvl)
{
	return abilities[ability_lvl]->getAbilityName();
}

bool AbilityGroup::useAbility(unsigned int idx, Vector2 cursor_pos)
{
	abilities[idx]->activate(cursor_pos);
	return abilities[idx]->hasParticleEffect();
}

bool AbilityGroup::IsSummoningHero(unsigned int idx)
{
	return abilities[idx]->summonsHero();
}

Engine::Emitter AbilityGroup::getEmitterData(unsigned int idx)
{
	return abilities[idx]->getEmitterData();
}

void AbilityGroup::initAbilities(AbilityCategories category)
{
	switch (category)
	{
	case AbilityCategories::PEOPLE:
	{
		category_name = "PEOPLE";
		for (auto& ability : abilities_peoples)
		{
			abilities.emplace_back(std::make_unique<People>(ability));
		}
	} break;
	case AbilityCategories::VEGETATION:
	{
		category_name = "VEGETATION";
		for (auto& ability : abilities_vegetation)
		{
			abilities.emplace_back(std::make_unique<Vegetation>(ability));
		}
	} break;
	case AbilityCategories::EARTH:
	{
		category_name = "EARTH";
		for (auto& ability : abilities_earth)
		{
			abilities.emplace_back(std::make_unique<Earth>(ability));
		}
	} break;
	case AbilityCategories::AIR:
	{
		category_name = "AIR";
		for (auto& ability : abilities_air)
		{
			abilities.emplace_back(std::make_unique<Air>(ability));
		}
	} break;
	case AbilityCategories::WATER:
	{
		category_name = "WATER";
		for (auto& ability : abilities_water)
		{
			abilities.emplace_back(std::make_unique<Water>(ability));
		}
	} break;
	case AbilityCategories::FIRE:
	{
		category_name = "FIRE";
		for (auto& ability : abilities_fire)
		{
			abilities.emplace_back(std::make_unique<Fire>(ability));
		}
	} break;
	case AbilityCategories::UNDEFINED:
	{
		std::cout << "This shouldn't have happened...\n";
	} break;
	}
}