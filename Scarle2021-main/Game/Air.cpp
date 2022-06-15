/// Created by Norbert Kupeczki

#include "pch.h"
#include "Air.h"

Air::Air(AbilitiesAir ability)
{
	initAbility(ability);
}

void Air::initAbility(AbilitiesAir ability)
{
	switch (ability)
	{
	case AbilitiesAir::LIGHTNING_BOLT:
	{
		ability_name = "Lightning bolt";
		mana_cost = 1;
	}
		break;
	case AbilitiesAir::LIGHTNING_STORM:
	{
		ability_name = "Lightning storm";
		mana_cost = 2;
	}
		break;
	case AbilitiesAir::WHIRLWIND:
	{
		ability_name = "Whirlwind";
		mana_cost = 3;
	}
		break;
	case AbilitiesAir::SUMMON_ODYSSEUS:
	{
		ability_name = "Summon Odysseus";
		mana_cost = 4;

		summons_hero = true;
		hero_type = Engine::HeroType::ODYSSEUS;
	}
		break;
	case AbilitiesAir::GALE_FORCE_WINDS:
	{
		ability_name = "Gale force winds";
		mana_cost = 5;
	}
		break;
	default:
		break;
	}
}
