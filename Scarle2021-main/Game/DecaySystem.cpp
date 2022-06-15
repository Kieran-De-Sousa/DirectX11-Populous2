#include "pch.h"

#include "DecaySystem.h"

#include <iostream>

Engine::DecaySystem::DecaySystem() : coordinator(nullptr)
{
	// Can't initialise coordinator in here because of header only circular dependency issues
}

void Engine::DecaySystem::init(Coordinator* _coordinator)
{
	coordinator = _coordinator;
}

void Engine::DecaySystem::update(const DX::StepTimer& timer)
{
	for (const auto& entity : entities)
	{
		EnergyDecay& decay_comp = coordinator->getComponent<EnergyDecay>(entity);
		Unit& unit_energy = coordinator->getComponent<Unit>(entity);

		unit_energy.energy -= decay_comp.decay_per_sec * timer.GetElapsedSeconds();
		if (unit_energy.energy <= 0.0F)
		{
			std::cout << entity << ": Died from energy loss." << std::endl;
			coordinator->destroyEntityAtEndOfFrame(entity);
		}
	}
}
