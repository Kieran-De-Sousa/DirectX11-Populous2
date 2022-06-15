#pragma once

#include "StepTimer.h"
#include <memory>

#include "System.h"
#include "Coordinator.h"
#include "TileMap.h"

#include "Transform.h"
#include "Settlement.h"
#include "Unit.h"
#include "AbilitySpawner.h"

#include "SharedData.h"
#include "MoreMath.h"

namespace Engine
{
	/// <summary>
	/// An AI system that creates Ability spawner entities 
	/// </summary>
	class AbilityAISystem : public Engine::System
	{
	public:
		AbilityAISystem();
		~AbilityAISystem() = default;
		/// <summary>
		/// Init the system, systems can't have constructors so required after creation
		/// </summary>
		/// <param name="_coordinator"> | Pointer to the scene's coordinator </param>
		/// <param name="_tile_map"> | Pointer to the scene's tile map</param>
		/// <param name="_ai_data"> | Data struct from SharedData.h</param>
		/// <param name="mana_pool"> | Pointer to the mana pool that the AI uses</param>
		/// <param name="_ai_team"> | The team the AI belongs to</param>
		void init(Coordinator* _coordinator, TileMap* _tile_map, AbilityAIData _ai_data, std::shared_ptr<float> mana_pool, Engine::Teams _ai_team);
		/// <summary>
		/// Updates the system, if the timer is up, weighs abilities, creates heatmap based on ability, selects a target tile, spawns an entity there and resets timer
		/// </summary>
		/// <param name="timer">Driver's->Scene's StepTimer</param>
		void update(const DX::StepTimer& timer);
		/// <summary>
		/// Adds a new ability the AI should consider
		/// </summary>
		/// <param name="new_ability">Ability info from SharedData.h</param>
		void addAbility(const AIAbilityInfo& new_ability);
		/// <summary>
		/// Clears thelist of abilities the AI has, useful to change abilities on the fly
		/// </summary>
		void clearAbilityList();
	private:
		/// <summary>
		/// Fills the floating point array with a standard value
		/// </summary>
		/// <param name="t_array">Pointer to float array</param>
		/// <param name="length">Float array size</param>
		/// <param name="value">Value to fill array with</param>
		void fill(float* t_array, unsigned length, float value);
		/// <summary>
		/// The team the AI belongs to
		/// </summary>
		Engine::Teams ai_team = Engine::Teams::BLUE;
		/// <summary>
		/// The data the AI was initialized with, potentionally changed through addAbility and clearAbilityList
		/// </summary>
		AbilityAIData ai_data;
		/// <summary>
		/// Pointer to the mana pool the AI uses
		/// </summary>
		std::shared_ptr<float> mana_ptr = nullptr;
		/// <summary>
		/// Pointer to the scene's coordinator
		/// </summary>
		Coordinator* coordinator = nullptr;
		/// <summary>
		/// Pointer to the scene's tilemap
		/// </summary>
		TileMap* tile_map = nullptr;
		/// <summary>
		/// AI's timer for waiting inbetween attempting using abilities
		/// </summary>
		float cast_timer = 5.0F;
	};
}
