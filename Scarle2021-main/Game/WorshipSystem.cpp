#include "pch.h"
#include "WorshipSystem.h"

Engine::WorshipSystem::WorshipSystem()
{
}

void Engine::WorshipSystem::init(Coordinator* _coordinator, float _mana_per_house)
{
	coordinator = _coordinator;
	mana_per_house = _mana_per_house;
}

void Engine::WorshipSystem::update(const DX::StepTimer& timer)
{
	for (auto& entity : entities)
	{
		//Get settlement component, diff mana gain for diff house types?
		auto& settlement = coordinator->getComponent<Settlement>(entity);
		for (auto& controller : controllers)
		{
			auto& control_comp = coordinator->getComponent<TeamController>(controller);
			//assert(coordinator->hasComponent<TeamController>(controller) && coordinator->hasComponent<ManaPool>(controller));
			if (settlement.building_team == control_comp.assigned_team)
			{
				*coordinator->getComponent<ManaPool>(controller).mana_pool += timer.GetElapsedSeconds() * mana_per_house;
			}
		}
	}
}

void Engine::WorshipSystem::registerController(const Entity& team_controller)
{
	controllers.push_back(team_controller);
}
