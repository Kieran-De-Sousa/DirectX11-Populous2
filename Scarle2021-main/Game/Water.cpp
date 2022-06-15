/// Created by Norbert Kupeczki

#include "pch.h"
#include "Water.h"

Water::Water(AbilitiesWater ability)
{
	initAbility(ability);
}

void Water::initAbility(AbilitiesWater ability)
{
	switch (ability)
	{
	case AbilitiesWater::BASALT_BARRIERS:
	{
		ability_name = "Create basalt barriers";
		mana_cost = 1;
	}
		break;
	case AbilitiesWater::WHIRLPOOL:
	{
		ability_name = "Whirlpool";
		mana_cost = 2;
	}
		break;
	case AbilitiesWater::BAPTISMAL_FONTS:
	{
		ability_name = "Baptismal fonts";
		mana_cost = 3;
	}
		break;
	case AbilitiesWater::SUMMON_HELEN:
	{
		ability_name = "Summon Helen";
		mana_cost = 4;

		summons_hero = true;
		hero_type = Engine::HeroType::HELEN;
	}
		break;
	case AbilitiesWater::TIDAL_WAVE:
	{
		ability_name = "Tidal wave";
		mana_cost = 5;
	}
		break;
	default:
		break;
	}
}
