// Created by Norbert Kupeczki

#pragma once

// A component to describe a hero unit
// It holds the type of the hero

namespace Engine
{
	enum class HeroType
	{
		UNDEFINED = 0,
		PERSEUS   = 1,
		ADONIS    = 2,
		HERAKLES  = 3,
		ODYSSEUS  = 4,
		ACHILLES  = 5,
		HELEN     = 6
	};

	struct Hero
	{
		Hero() = default;
		~Hero() = default;

		HeroType hero_type = HeroType::UNDEFINED;
	};
}