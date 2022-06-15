/// Created by Norbert Kupeczki

#include "pch.h"
#include "People.h"

People::People(AbilitiesPeople ability)
{
	initAbility(ability);
}

void People::initAbility(AbilitiesPeople ability)
{
	switch (ability)
	{
	case AbilitiesPeople::RAISE_LOWER_GROUND:
	{
		ability_name = "Raise / Lower ground";
		mana_cost = 1;
	}
		break;
	case AbilitiesPeople::PAPAL_MAGNET:
	{
		ability_name = "Place Papal magnet";
		mana_cost = 2;
	}
		break;
	case AbilitiesPeople::SUMMON_PERSEUS:
	{
		ability_name = "Summon Perseus";
		mana_cost = 3;

		summons_hero = true;
		hero_type = Engine::HeroType::PERSEUS;
	}
		break;
	case AbilitiesPeople::PLAGUE:
	{
		ability_name = "Plague";
		mana_cost = 4;
	}
		break;
	case AbilitiesPeople::ARMAGEDDON:
	{
		ability_name = "Armageddon";
		mana_cost = 5;
	}
		break;
	default:
		break;
	}
}
