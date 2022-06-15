
#include "pch.h"
#include "FlagRaisingSystem.h"

Engine::FlagRaisingSystem::FlagRaisingSystem() : coordinator(nullptr)
{
	// Can't initialise coordinator in here because of header only circular dependency issues
}

void Engine::FlagRaisingSystem::init(Coordinator* _coordinator)
{
	coordinator = _coordinator;
}

void Engine::FlagRaisingSystem::update(const DX::StepTimer& timer)
{
	for (const auto& entity : entities)
	{
		// Take relevant components 
		EnergyFlagComponent& flag_comp = coordinator->getComponent<EnergyFlagComponent>(entity);
		Transform& flag_trans = coordinator->getComponent<Transform>(entity);
		Entity& settlement = flag_comp.building;
		Transform& settlement_trans = coordinator->getComponent<Transform>(settlement);
		Energy& energy = coordinator->getComponent<Energy>(settlement);

		// Do maths
		flag_comp.energy_ratio = energy.amount / energy.max_amount;
		DirectX::SimpleMath::Vector3 lifting = { 0, flag_comp.lift_height * flag_comp.energy_ratio, 0 };
		
		// Set position
		flag_trans.position = settlement_trans.position + flag_comp.offset - lifting;
	}
}
