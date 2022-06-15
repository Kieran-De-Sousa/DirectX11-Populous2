/// Created by Norbert Kupeczki

#pragma once

#include <iostream>
#include <string>
#include <SimpleMath.h>
#include "AbilityData.h"
#include "Emitter.h"
#include "Hero.h"

using Vector2 = DirectX::SimpleMath::Vector2;

class Ability
{
public:
	Ability() = default;
	~Ability() = default;

	/// <summary>
	/// A function to get the name of the ability
	/// </summary>
	/// <returns>
	/// Returns a string with the name of the ability
	/// </returns>
	std::string getAbilityName();

	/// <summary>
	/// Activates the ability
	/// </summary>
	/// <param name="cursor_pos"></param>
	void activate(Vector2 cursor_pos);

	/// <summary>
	/// Getter function to get whether the ability has a particle effect component
	/// </summary>
	/// <returns>
	/// Returns true if the ability has to create a particle effect.
	/// </returns>
	bool hasParticleEffect();

	/// <summary>
	/// A getter function to get whether the ability summons a hero
	/// </summary>
	/// <returns>
	/// Returns true of the ability summons a hero.
	/// </returns>
	bool summonsHero();

	/// <summary>
	/// A getter function to return the data that is needed to create a particle system
	/// </summary>
	/// <returns>
	/// Returns an 'Emitter' type data struct
	/// </returns>
	Engine::Emitter getEmitterData();

protected:
	std::string ability_name;
	unsigned int mana_cost = 0;

	bool has_particle_effect = false;
	bool summons_hero = false;
	Engine::HeroType hero_type = Engine::HeroType::UNDEFINED;

	Engine::Emitter emitter_data;
};

