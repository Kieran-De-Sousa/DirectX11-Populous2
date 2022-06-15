#include "pch.h"
#include "AIAbilitySystem.h"

Engine::AIAbilitySystem::AIAbilitySystem()
{

}

void Engine::AIAbilitySystem::init(Coordinator* _coordinator, AbilityManager* _ability_manager, TileMap* _tile_map, EmitterSystem* _emitter_system)
{
	coordinator = _coordinator;
	ability_manager = _ability_manager;
	tile_map = _tile_map;
	emitter_system = _emitter_system;
}

// The update scans the entities list, and if it finds an entity that has an AbilitySpawner component,
// it updates its timer, and once it reaches 0, it uses the ability it "holds", then destroys the entity.

void Engine::AIAbilitySystem::update(const DX::StepTimer& timer)
{
	for (const Entity& entity : entities)
	{
		AbilitySpawner& spawner = coordinator->getComponent<AbilitySpawner>(entity);
		Transform& transform = coordinator->getComponent<Transform>(entity);

		if (spawner.delay -= timer.GetElapsedSeconds() <= 0.0F)
		{
			convertAbilityAndSkillLevel(spawner.ability);
			useAbility(transform, ai_cat, ai_ability_lvl);
			coordinator->destroyEntityAtEndOfFrame(entity);
		}
	}
}

// Checks whether the ability needs to be used is an ability that turns a leader into a hero,
// or an ability that creates a particle effect.

void Engine::AIAbilitySystem::useAbility(const Transform& transform, AbilityCategories category, unsigned int ability_lvl)
{
	Vector2 pos2d = Vector2(transform.position.x, transform.position.y);  // Uses the "beacon" entity's transform to get the location

	if (ability_manager->IsAISummoningHero(category, ability_lvl))			 // If the ability the AI wants to use is a hero skill, signals the
																															     // "Leader system" to promote the leader to the selected hero.
	{
		std::cout << "AI USED A HERO SUMMONING ABILITY";
	}
	else if (ability_manager->AIUseAbility(pos2d, category, ability_lvl))	 // If it is an ability that requires a particle system, it pulls the 
	{																															 // data from the Ability Manager and creates the emitter.
		Engine::Emitter emitter = ability_manager->AIgetEmitterData(category, ability_lvl);
		emitter.location = pos2d;

		Vector3 velocity(0, 0, 0);																				 // If we want a moving emitter, we need to add velocity to it
		emitter_system->createEmitter(emitter, velocity);
	}
}

// A function that converts the ability ID the AI selected to cast to data that the
// existing ability manager system can use.
// The AI data is an unsigned integer between 0 and 29.

void Engine::AIAbilitySystem::convertAbilityAndSkillLevel(AbilityTypes to_convert)
{
	unsigned int x = static_cast<unsigned int>(to_convert);
	ai_ability_lvl =  x % 5;

	if (0 <= x && x <= 4)
	{
		ai_cat = AbilityCategories::PEOPLE;
	}
	else if (5 <= x && x <= 9)
	{
		ai_cat = AbilityCategories::VEGETATION;
	}
	else if(10 <= x && x <= 14)
	{
		ai_cat = AbilityCategories::EARTH;
	}
	else if (15 <= x && x <= 19)
	{
		ai_cat = AbilityCategories::AIR;
	}
	else if (20 <= x && x <= 24)
	{
		ai_cat = AbilityCategories::WATER;
	}
	else
	{
		ai_cat = AbilityCategories::FIRE;
	}
}