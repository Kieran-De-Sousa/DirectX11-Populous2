#include "pch.h"

#include "AISettlementWatcherSystem.h"
#include <iostream>

Engine::AISettlementWatchSystem::AISettlementWatchSystem()
{
}

void Engine::AISettlementWatchSystem::init(Coordinator* _coordinator)
{
	coordinator = _coordinator;
}

void Engine::AISettlementWatchSystem::update(const DX::StepTimer& timer)
{
	if (enabled)
	{
		for (auto& entity : entities)
		{
			for (auto& ai : ai_data)
			{
				Settlement& settlement_comp = coordinator->getComponent<Settlement>(entity);
				Energy& settlement_energy = coordinator->getComponent<Energy>(entity);
				//If it should add a watch
				if (ai.ai_team == settlement_comp.building_team && settlement_energy.amount / settlement_energy.max_amount > ai.energy_ratio_threshold)
				{
					/* YAY FOR HASCOMPONENT
					bool only_once = true;
					for (auto& already_watched : on_watch)
					{
						if (already_watched.first == entity)
						{
							only_once = false;
						}
					}
					if (only_once)
					{
						float sprog_timer = ai_data.min_wait + (ai_data.wait_variation * (float)rand() / ((float)RAND_MAX + 1));
						on_watch.push_back({ entity, sprog_timer });
					}
					*/
					//And doesnt have a watch yet
					if (!coordinator->hasComponent<SettlementAIWatch>(entity))
					{
						//Then add a watch
						auto& new_watch = coordinator->addComponent<SettlementAIWatch>(entity, {});
						new_watch.timer = ai.min_wait + (ai.wait_variation * (float)rand() / ((float)RAND_MAX + 1));
					}
				}
				// But if it's instructed to instantly sprog at max energy, then do that and ensure there's no watch
				if (ai.ai_team == settlement_comp.building_team && ai.insta_sprog_at_max)
				{
					if (settlement_energy.amount == settlement_energy.max_amount)
					{
						std::cout << entity << ": Max Energy." << std::endl;
						settlement_comp.sprogged = true;
						coordinator->removeComponent<SettlementAIWatch>(entity);
					}
				}
			}
		}
	}
}

void Engine::AISettlementWatchSystem::disableAI()
{
	enabled = false;
	//Remove all watch components? Separate pause and remove AI?
}

void Engine::AISettlementWatchSystem::enableAI()
{
	enabled = true;
}

void Engine::AISettlementWatchSystem::addTeamAI(SprogAIData new_ai_data)
{
	ai_data.push_back(new_ai_data);
}
