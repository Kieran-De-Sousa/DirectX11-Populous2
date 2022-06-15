// Createad by Norbert 03/05/2022

#pragma once

#include "System.h"
#include "Coordinator.h"
#include "StepTimer.h"
#include "SharedData.h"

#include "Transform.h"
#include "AbilityManager.h"
#include "EmitterSystem.h"
#include "AbilitySpawner.h"
#include "TileMap.h"
#include "Emitter.h"

namespace Engine
{
	class AIAbilitySystem : public Engine::System
	{
	public:
		AIAbilitySystem();
		~AIAbilitySystem() = default;

		/// <summary>
		/// Initialiser to create the links to other systems: Ability manager, Tile Map and Emitter system.
		/// </summary>
		/// <param name="_coordinator pointer"></param>
		/// <param name="_ability_manager pointer"></param>
		/// <param name="_tile_map pointer"></param>
		/// <param name="_emitter_system pointer"></param>
		void init(Coordinator* _coordinator, AbilityManager* _ability_manager, TileMap* _tile_map, EmitterSystem* _emitter_system);
		void update(const DX::StepTimer& timer);

	private:
		/// <summary>
		/// A function to invoke an ability used by the AI
		/// </summary>
		/// <param name="transform"></param>
		/// <param name="category"></param>
		/// <param name="ability_lvl"></param>
		void useAbility(const Transform& transform, AbilityCategories category, unsigned int ability_lvl);

		/// <summary>
		/// A function to convert the AI ability ID to data types that the Ability manager can understand.
		/// </summary>
		/// <param name="to_convert"></param>
		void convertAbilityAndSkillLevel(AbilityTypes to_convert);

		Coordinator* coordinator = nullptr;
		AbilityManager* ability_manager = nullptr;
		TileMap* tile_map = nullptr;
		EmitterSystem* emitter_system = nullptr;

		AbilityCategories ai_cat = AbilityCategories::FIRE;
		unsigned int ai_ability_lvl = 0;
	};
}
