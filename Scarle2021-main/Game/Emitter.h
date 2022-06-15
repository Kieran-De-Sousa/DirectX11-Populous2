// Created by Norbert on 21/03/2022

#pragma once

// A component to describe an emitter that will be used
// to summon particles once it is placed in the game world

using Vector3 = DirectX::SimpleMath::Vector3;
using Vector2 = DirectX::SimpleMath::Vector2;

namespace Engine
{
	enum class EmitterTypes
	{
		UNDEFINED  = 0,
		HORIZONTAL = 1,
		VERTICAL   = 2,
		TILE	   = 3
	};

	enum class DamageType
	{
		NO_DAMAGE		  = 0,
		DAMAGING_EMITTER  = 1,
		DAMAGING_PARTICLE = 2,
		DAMAGING_BOTH	  = 3
	};

	struct Emitter
	{
		Emitter() = default;
		~Emitter() = default;

		EmitterTypes particle_type = EmitterTypes::UNDEFINED;
		DamageType damage_type = DamageType::NO_DAMAGE;
		Vector2 location {0.0F, 0.0F};

		unsigned int emission_speed = 1;
		float emission_cd = 0.0F;

		std::string sprite = "test";

		float emitter_lifetime = 0.0F;
		float particle_lifetime = 0.0F;
		float tile_spread = 0.0F;
		float particle_speed = 0.0F;

		bool world_space_particles = false;
		float damage = 0.0F;
	};
}