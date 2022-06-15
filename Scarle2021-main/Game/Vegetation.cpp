/// Created by Norbert Kupeczki

#include "pch.h"
#include "Vegetation.h"

Vegetation::Vegetation(AbilitiesVegetation ability)
{
	initAbility(ability);
}

void Vegetation::initAbility(AbilitiesVegetation ability)
{
	switch (ability)
	{
	case AbilitiesVegetation::CREATE_TREES:
	{
		ability_name = "Create trees";
		mana_cost = 1;
	}
		break;
	case AbilitiesVegetation::FLORAL_BLOOM:
	{
		ability_name = "Floral bloom";
		mana_cost = 2;
	}
		break;
	case AbilitiesVegetation::SWAMP:
	{
		ability_name = "Swamp";
		mana_cost = 3;
	}
		break;
	case AbilitiesVegetation::DEADLY_FUNGUS:
	{
		ability_name = "Deadly fungus";
		mana_cost = 4;
	}
		break;
	case AbilitiesVegetation::SUMMON_ADONIS:
	{
		ability_name = "Summon Adonis";
		mana_cost = 5;

		summons_hero = true;
		hero_type = Engine::HeroType::ADONIS;
	}
		break;
	default:
		break;
	}
}
