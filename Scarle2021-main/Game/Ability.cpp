/// Created by Norbert Kupeczki

#include "pch.h"
#include "Ability.h"

std::string  Ability::getAbilityName()
{
	return ability_name;
}

void Ability::activate(Vector2 cursor_pos)
{
	std::cout << ability_name + " used at " + std::to_string(cursor_pos.x) + " : " + std::to_string(cursor_pos.y) + ", cost: " + std::to_string(mana_cost) + " mana\n";
}

bool Ability::hasParticleEffect()
{
	return has_particle_effect;
}

bool Ability::summonsHero()
{
	return summons_hero;
}

Engine::Emitter Ability::getEmitterData()
{
	return emitter_data;
}
