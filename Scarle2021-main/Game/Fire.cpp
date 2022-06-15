/// Created by Norbert Kupeczki

#include "pch.h"
#include "Fire.h"

Fire::Fire(AbilitiesFire ability)
{
	initAbility(ability);
}

void Fire::initAbility(AbilitiesFire ability)
{
	switch (ability)
	{
	case AbilitiesFire::PILLAR_OF_FIRE:
	{
		ability_name = "Pillar of Fire";
		mana_cost = 1;

		has_particle_effect = true;
		emitter_data.particle_type = Engine::EmitterTypes::VERTICAL;
		emitter_data.damage_type = Engine::DamageType::DAMAGING_EMITTER;
		emitter_data.emission_speed = 20;
		emitter_data.sprite = "fire";
		emitter_data.emitter_lifetime = 3.0F;
		emitter_data.particle_lifetime = 0.75F;
		emitter_data.particle_speed = 100.0F;
		emitter_data.damage = 50.0F;
	}
		break;
	case AbilitiesFire::FIRE_STORM:
	{
		ability_name = "Fire storm";
		mana_cost = 2;

		has_particle_effect = true;
		emitter_data.particle_type = Engine::EmitterTypes::TILE;
		emitter_data.damage_type = Engine::DamageType::DAMAGING_PARTICLE;
		emitter_data.emission_speed = 20;
		emitter_data.sprite = "fire";
		emitter_data.emitter_lifetime = 6.0F;
		emitter_data.particle_lifetime = 1.25F;
		emitter_data.tile_spread = 80.0F;
		emitter_data.damage = 25.0F;
	}
		break;
	case AbilitiesFire::SUMMON_ACHILLES:
	{
		ability_name = "Summon Achilles";
		mana_cost = 3;

		summons_hero = true;
		hero_type = Engine::HeroType::ACHILLES;
	}
		break;
	case AbilitiesFire::VOLCANO:
	{
		ability_name = "Volcano";
		mana_cost = 4;
	}
		break;
	case AbilitiesFire::FIRE_STREAMS:
	{
		ability_name = "Fire streams";
		mana_cost = 5;
	}
		break;
	default:
		break;
	}
}
