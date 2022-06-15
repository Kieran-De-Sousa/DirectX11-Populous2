
#include "pch.h"
#include "FlagCreationSystem.h"

Engine::FlagCreationSystem::FlagCreationSystem() : coordinator(nullptr), renderer(nullptr)
{
	// Can't initialise coordinator in here because of header only circular dependency issues
}

void Engine::FlagCreationSystem::init(Coordinator* _coordinator, RenderSystem* _renderer)
{
	coordinator = _coordinator;
	renderer = _renderer;
}

void Engine::FlagCreationSystem::update(const DX::StepTimer& timer)
{
	for (const auto& entity : entities)
	{
		Settlement& settlement = coordinator->getComponent<Settlement>(entity);
		// If settlement has uninitialized flag
		if (settlement.flag_entity == NULL_ENTITY)
		{
			Energy& settlement_energy = coordinator->getComponent<Energy>(entity);
			Entity flag_ent = coordinator->createChild(entity);

			// Create flag entity
			settlement.flag_entity = flag_ent;
			EnergyFlagComponent& flag_comp = coordinator->addComponent<EnergyFlagComponent>(flag_ent, {});
			flag_comp.building = entity;
			flag_comp.offset = { 18, 18, 0 }; // magic offset
			//coordinator->getComponent<SpriteRenderer>(entity).  --> take the house sprite size out to create offset, or maybe use SharedData or Settlement comp?
			flag_comp.energy_ratio = settlement_energy.amount / settlement_energy.max_amount;

			// Position flag entity
			DirectX::SimpleMath::Vector3 lifting;
			lifting = { 0, flag_comp.lift_height*flag_comp.energy_ratio, 0 };
			Transform& flag_trans = coordinator->addComponent<Transform>(flag_ent, {});
			flag_trans.position = coordinator->getComponent<Transform>(entity).position + flag_comp.offset - lifting;

			// Flag Sprite
			SpriteRenderer& flag_sprite = coordinator->addComponent<SpriteRenderer>(flag_ent, {});
			if (settlement.building_team == Teams::RED)
			{
				renderer->initSprite("red_flag", flag_ent);
			}
			else
			{
				renderer->initSprite("blue_flag", flag_ent);
			}
			
		}
	}
}
