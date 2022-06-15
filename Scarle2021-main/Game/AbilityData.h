/// Created by Norbert Kupeczki

#pragma once

#include <vector>

enum class AbilityCategories : int
{
	PEOPLE = 0,
	VEGETATION = 1,
	EARTH = 2,
	AIR = 3,
	WATER = 4,
	FIRE = 5,
	UNDEFINED = 6
};

enum class AbilitiesPeople : int
{
	RAISE_LOWER_GROUND = 0,
	PAPAL_MAGNET = 1,
	SUMMON_PERSEUS = 2,
	PLAGUE = 3,
	ARMAGEDDON = 4
};

enum class AbilitiesVegetation : int
{
	CREATE_TREES = 0,
	FLORAL_BLOOM = 1,
	SWAMP = 2,
	DEADLY_FUNGUS = 3,
	SUMMON_ADONIS = 4
};

enum class AbilitiesEarth : int
{
	BUILD_ROAD = 0,
	BUILD_WALL = 1,
	EARTHQUAKE = 2,
	CREATE_MOUNTAIN = 3,
	SUMMON_HERAKLES = 4
};

enum class AbilitiesAir : int
{
	LIGHTNING_BOLT = 0,
	LIGHTNING_STORM = 1,
	WHIRLWIND = 2,
	SUMMON_ODYSSEUS = 3,
	GALE_FORCE_WINDS = 4
};

enum class AbilitiesWater : int
{
	BASALT_BARRIERS = 0,
	WHIRLPOOL = 1,
	BAPTISMAL_FONTS = 2,
	SUMMON_HELEN = 3,
	TIDAL_WAVE = 4
};

enum class AbilitiesFire : int
{
	PILLAR_OF_FIRE = 0,
	FIRE_STORM = 1,
	SUMMON_ACHILLES = 2,
	VOLCANO = 3,
	FIRE_STREAMS = 4
};