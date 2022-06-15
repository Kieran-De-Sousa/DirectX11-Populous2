#pragma once

#include <unordered_map>
#include <map>
#include <SimpleMath.h>

namespace Engine
{
	/// <summary>
	/// Teams that the game supports
	/// </summary>
	enum class Teams
	{
		BLUE,
		RED,
		NUM_OF_TEAMS
	};

	/// <summary>
	/// Types of units in the game
	/// </summary>
	enum class PawnTypes
	{
		NORMAL,
		HERCULES
	};
	
	/// <summary>
	/// Types of houses in the game
	/// </summary>
	enum class BuildingTypes
	{
		HUT,
		HOUSE,
		VILLAGE,
		TOWN,
		CASTLE,
		NUM_OF_TYPES
	};

	/// <summary>
	/// Types of tiles in the game (TileMap::Tile::TileState should be moved up here)
	/// </summary>
	enum class TileTypes
	{
		GRASS,
		WATER,
		MOUNTAIN,
		NON_WALKABLE,
		NUM_OF_TYPES
	};

	/// <summary>
	/// A list of abilities
	/// </summary>
	enum class AbilityTypes
	{
		RAISE_LOWER_GROUND,
		PAPAL_MAGNET,
		SUMMON_PERSEUS,
		PLAGUE,
		ARMAGEDDON,
		CREATE_TREES,
		FLORAL_BLOOM,
		SWAMP, 
		DEADLY_FUNGUS,
		SUMMON_ADONIS,
		BUILD_ROAD,
		BUILD_WALL,
		EARTHQUAKE,
		CREATE_MOUNTAIN,
		SUMMON_HERAKLES,
		LIGHTNING_BOLT,
		LIGHTNING_STORM,
		WHIRLWIND,
		SUMMON_ODYSSEUS,
		GALE_FORCE_WINDS,
		BASALT_BARRIERS,
		WHIRLPOOL,
		BAPTISMAL_FONTS,
		SUMMON_HELEN,
		TIDAL_WAVE,
		PILLAR_OF_FIRE,
		FIRE_STORM,
		SUMMON_ACHILLES,
		VOLCANO,
		FIRE_STREAMS
	};

	/// <summary>
	/// An ability info structure for the Ability AI to work with, create a weight for the ability, and construct a heatmap with it if selected
	/// </summary>
	struct AIAbilityInfo
	{
		/// <summary>
		/// Ability if selected
		/// </summary>
		AbilityTypes ability = AbilityTypes::RAISE_LOWER_GROUND;
		/// <summary>
		/// Ability's mana cost
		/// </summary>
		float mana_cost = 1.0F;
		/// <summary>
		/// Standard delay before the spawned entity is used by the system to create the ability effect
		/// </summary>
		float cast_delay = 0.0F;
		/// <summary>
		/// Max random delay to add on top of the standard delay
		/// </summary>
		float cast_delay_variability = 3.0F;
		/// <summary>
		/// The size of the segments to divide the tile map into
		/// X by X, defaults to one, creating a heatmap value for each tile
		/// </summary>
		int segmentation = 1;
		/// <summary>
		/// Accuracy as of yet unused
		/// </summary>
		DirectX::SimpleMath::Vector2 accuracy_x = { -2,2 };
		DirectX::SimpleMath::Vector2 accuracy_y = { -2,2 };
		//Make a distance based accuracy? for + shape patterns?

		/// <summary>
		/// Ability choice variables
		/// Modifiers added together, modifiers multiplied together
		/// Then the total modifier is multiplied by the total multiplier
		/// A random ability is then selected, higher weight is more likely
		/// </summary>
		float ability_weight_marker = 0.0F;

		float ab_weight_multiplier = 1.0F;
		float ab_weight_modifier = 1.0F;

		float mana_multiplier = 0.8F;
		float mana_modifier = 1.0F;

		float ab_own_unit_mult = 1.25F;
		float ab_own_unit_mod = 1.0F;

		float ab_oth_unit_mult = 1.0F;
		float ab_oth_unit_mod = 1.0F;

		float ab_own_settlement_mult = 0.0F;
		float ab_own_settlement_mod = 1.0F;

		float ab_oth_settlement_mult = 0.0F;
		float ab_oth_settlement_mod = 1.0F;
		
		/// <summary>
		/// Weight Map / Heat Map variables
		/// Works similar to the ability choice variables
		/// </summary>
		float empty_segment_ticket = 0.1F;

		float map_own_unit_mult = 1.25F;
		float map_own_unit_mod = 1.0F;

		float map_oth_unit_mult = 1.0F;
		float map_oth_unit_mod = 1.0F;

		float map_own_settlement_mult = 0.0F;
		float map_own_settlement_mod = 1.0F;

		float map_oth_settlement_mult = 0.0F;
		float map_oth_settlement_mod = 1.0F;
	};

	/// <summary>
	/// Data for defining the Ability AI behaviour
	/// By default no abilities are assigned to it
	/// </summary>
	struct AbilityAIData
	{
		/// <summary>
		/// List of abilities
		/// </summary>
		std::vector<AIAbilityInfo> list_of_abilities;
		/// <summary>
		/// Standard wait inbetween abilities
		/// </summary>
		float standard_wait = 5.0F;
		/// <summary>
		/// Max randomized wait to add to standard amount
		/// </summary>
		float random_wait = 5.0F;
		/// <summary>
		/// Past mana threshold decrease wait time by multiplying it with this value
		/// </summary>
		float mana_wait_mod = 0.5F;
		/// <summary>
		/// Threshold to decrease wait time between spells at
		/// </summary>
		float mana_mod_threshold = 100.0F;
		/// <summary>
		/// Modify the cost of spells by multiplying it with this value, both when considering using it and spending mana on it
		/// </summary>
		float mana_cost_mod = 1.0F;
	};

	/// <summary>
	/// Data for defining the Sprogging AI's behaviour
	/// </summary>
	struct SprogAIData
	{
		/// <summary>
		/// Team to work for
		/// </summary>
		Engine::Teams ai_team = Engine::Teams::BLUE;
		/// <summary>
		/// Minimum time to wait on a failed attempt
		/// </summary>
		float min_wait = 0.0f;
		/// <summary>
		/// Max random time to add to minimum wait time
		/// </summary>
		float wait_variation = 2.0f;
		/// <summary>
		/// The current energy / max energy ratio to start watching settlements
		/// 1.0F = Full energy
		/// 0.25F = 25% energy
		/// </summary>
		float energy_ratio_threshold = 0.25f;
		/// <summary>
		/// Chance to successfully sprog settlement at the threshold energy
		/// 1.0F = 100%
		/// 0.1F = 10%
		/// </summary>
		float threshold_chance = 0.1f;
		/// <summary>
		/// Chance to successfully sprog settlement at max energy
		/// Between threshold and max energy the chance is linearly scaling
		/// 1.0F = 100%
		/// 0.1F = 10%
		/// </summary>
		float max_chance = 0.5f;
		/// <summary>
		/// Whether the AI should sprog settlements that are at max energy without rolling
		/// </summary>
		bool insta_sprog_at_max = false;
	};

	//this is working

	/// <summary>
	/// Mostly unused, a handy way of storing some data 
	/// </summary>
	class TypeMaps
	{
	public:
		TypeMaps()
		{
			// Check if maps are filled
			// Add default values if they arent
			// How do you iterate through enums? It's a bit trickys
			// Found answer: use Num_of_types at the end for a for loop
		};
		~TypeMaps() = default;
		std::map<TileTypes, bool> BuildableData = 
		{ {std::make_pair<TileTypes, bool>(TileTypes::GRASS, true)}, 
		{std::make_pair<TileTypes, bool>(TileTypes::WATER, false)},
		{std::make_pair<TileTypes, bool>(TileTypes::MOUNTAIN, false)},
		{std::make_pair<TileTypes, bool>(TileTypes::NON_WALKABLE, false)},
		};

		std::map<BuildingTypes, float> BuildingGeneration =
		{ {std::make_pair<BuildingTypes, float>(BuildingTypes::HUT, 0.5F)},
		{std::make_pair<BuildingTypes, float>(BuildingTypes::HOUSE, 0.7F)}
		};
	};
}