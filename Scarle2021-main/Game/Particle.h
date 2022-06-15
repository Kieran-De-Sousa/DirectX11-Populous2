// Created by Norbert on 21/03/2022

#pragma once

// A component for the individual particle properties

namespace Engine
{
	enum class ParticleTypes
	{
		UNDEFINED  = 0,
		HORIZONTAL = 1,
		VERTICAL   = 2,
		TILE       = 3
	};

	struct Particle
	{
		Particle() = default;
		~Particle() = default;

		ParticleTypes particle_type = ParticleTypes::UNDEFINED;

		bool damaging = false;
		float damage = 0.0F;
	};
}