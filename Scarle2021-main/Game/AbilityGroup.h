/// Created by Norbert Kupeczki

//#pragma once

#include <vector>
#include <SimpleMath.h>
#include "Ability.h"
#include "AbilityData.h"
#include "Emitter.h"

using Vector2 = DirectX::SimpleMath::Vector2;

class AbilityGroup
{
public:
	AbilityGroup(AbilityCategories category);
	~AbilityGroup() = default;

	/// <summary>
	/// A mutator function to get the name of an ability category
	/// </summary>
	/// <returns>
	/// Returns a string with the name of the ability group.
	/// </returns>
	std::string getCategoryName();

	/// <summary>
	/// A mutator function to get the name of an ability in a category
	/// </summary>
	/// <param name="ability_lvl"></param>
	/// <returns>
	/// Returns the name of the ability.
	/// </returns>
	std::string getAbilityName(unsigned int ability_lvl);

	/// <summary>
	/// A function to invoke an ability at a certain location
	/// </summary>
	/// <param name="idx"></param>
	/// <param name="cursor_pos"></param>
	/// <returns>
	/// Returns true if the ability has a particle system component.
	/// </returns>
	bool useAbility(unsigned int idx, Vector2 cursor_pos);

	/// <summary>
	/// Checking whether the ability summons a hero.
	/// </summary>
	/// <param name="idx"></param>
	/// <returns>
	/// Returns true if the 
	/// </returns>
	bool IsSummoningHero(unsigned int idx);

	/// <summary>
	/// Getter function to get the data from an ability to create a particle system
	/// </summary>
	/// <param name="idx"></param>
	/// <returns>
	/// Returns an 'Emitter' type data struct with the data needed to create a particle system
	/// </returns>
	Engine::Emitter getEmitterData(unsigned int idx);

private:
	std::string category_name = "default name";

	std::vector<std::unique_ptr<Ability>> abilities;

	void initAbilities(AbilityCategories category);

	std::vector<AbilitiesPeople> abilities_peoples =
	{
		AbilitiesPeople::RAISE_LOWER_GROUND,
		AbilitiesPeople::PAPAL_MAGNET,
		AbilitiesPeople::SUMMON_PERSEUS,
		AbilitiesPeople::PLAGUE,
		AbilitiesPeople::ARMAGEDDON
	};

	std::vector<AbilitiesVegetation> abilities_vegetation =
	{
		AbilitiesVegetation::CREATE_TREES,
		AbilitiesVegetation::FLORAL_BLOOM,
		AbilitiesVegetation::SWAMP,
		AbilitiesVegetation::DEADLY_FUNGUS,
		AbilitiesVegetation::SUMMON_ADONIS
	};

	std::vector<AbilitiesEarth> abilities_earth =
	{
		AbilitiesEarth::BUILD_ROAD,
		AbilitiesEarth::BUILD_WALL,
		AbilitiesEarth::EARTHQUAKE,
		AbilitiesEarth::CREATE_MOUNTAIN,
		AbilitiesEarth::SUMMON_HERAKLES
	};

	std::vector<AbilitiesAir> abilities_air =
	{
		AbilitiesAir::LIGHTNING_BOLT,
		AbilitiesAir::LIGHTNING_STORM,
		AbilitiesAir::WHIRLWIND,
		AbilitiesAir::SUMMON_ODYSSEUS,
		AbilitiesAir::GALE_FORCE_WINDS
	};

	std::vector<AbilitiesWater> abilities_water =
	{
		AbilitiesWater::BASALT_BARRIERS,
		AbilitiesWater::WHIRLPOOL,
		AbilitiesWater::BAPTISMAL_FONTS,
		AbilitiesWater::SUMMON_HELEN,
		AbilitiesWater::TIDAL_WAVE
	};

	std::vector<AbilitiesFire> abilities_fire =
	{
		AbilitiesFire::PILLAR_OF_FIRE,
		AbilitiesFire::FIRE_STORM,
		AbilitiesFire::SUMMON_ACHILLES,
		AbilitiesFire::VOLCANO,
		AbilitiesFire::FIRE_STREAMS
	};
};

