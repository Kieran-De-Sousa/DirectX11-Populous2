/// Created by Norbert Kupeczki

#include "pch.h"
#include "Earth.h"

Earth::Earth(AbilitiesEarth ability)
{
	initAbility(ability);
}

void Earth::initAbility(AbilitiesEarth ability)
{
	switch (ability)
	{
	case AbilitiesEarth::BUILD_ROAD:
	{
		ability_name = "Build road";
		mana_cost = 1;
	}
		break;
	case AbilitiesEarth::BUILD_WALL:
	{
		ability_name = "Build wall";
		mana_cost = 2;
	}
		break;
	case AbilitiesEarth::EARTHQUAKE:
	{
		ability_name = "Earthquake";
		mana_cost = 3;
	}
		break;
	case AbilitiesEarth::CREATE_MOUNTAIN:
	{
		ability_name = "Create mountain";
		mana_cost = 4;
	}
		break;
	case AbilitiesEarth::SUMMON_HERAKLES:
	{
		ability_name = "Summon Herakles";
		mana_cost = 5;

		summons_hero = true;
		hero_type = Engine::HeroType::HERAKLES;
	}
		break;
	default:
		break;
	}
}
