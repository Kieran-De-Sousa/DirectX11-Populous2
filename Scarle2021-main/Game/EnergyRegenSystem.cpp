#include "pch.h"

#include "EnergyRegenSystem.h"

#include <iostream>
//testing
#include "SpriteRenderer.h"

Engine::EnergyRegenSystem::EnergyRegenSystem() : coordinator(nullptr)
{
	// Can't initialise coordinator in here because of header only circular dependency issues
}

void Engine::EnergyRegenSystem::init(Coordinator* _coordinator)
{
	coordinator = _coordinator;
}

void Engine::EnergyRegenSystem::update(const DX::StepTimer& timer)
{
	for (const auto& entity : entities)
	{
		Settlement& settlement_comp = coordinator->getComponent<Settlement>(entity);
		Energy& energy_comp = coordinator->getComponent<Energy>(entity);

		energy_comp.amount += settlement_comp.energy_per_sec * timer.GetElapsedSeconds();

		if (energy_comp.amount > energy_comp.max_amount)
		{
			energy_comp.amount = energy_comp.max_amount;
			
			//for testing:
			/*float r = 0.2F;
			float g = 1.0F;
			float b = 0.2F;
			coordinator->getComponent<Engine::SpriteRenderer>(settlement_comp.flag_entity).colour = DirectX::SimpleMath::Color(r, g, b, 1);*/
		}
	}
}
