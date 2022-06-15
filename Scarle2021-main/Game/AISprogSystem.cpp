#include "pch.h"

#include "AISprogSystem.h"

#include <iostream>

Engine::AISprogSystem::AISprogSystem() : coordinator(nullptr)
{
	// Can't initialise coordinator in here because of header only circular dependency issues
}

void Engine::AISprogSystem::init(Coordinator* _coordinator)
{
	coordinator = _coordinator;
}

void Engine::AISprogSystem::update(const DX::StepTimer& timer)
{
	if (enabled)
	{
		for (auto& watched_settlement : entities)
		{
			for (auto& ai : ai_data)
			{
				Settlement& settlement_comp = coordinator->getComponent<Settlement>(watched_settlement);
				if (ai.ai_team == settlement_comp.building_team && coordinator->hasComponent<SettlementAIWatch>(watched_settlement))
				{
					
					Energy& settlement_energy = coordinator->getComponent<Energy>(watched_settlement);
					SettlementAIWatch& settlement_watch = coordinator->getComponent<SettlementAIWatch>(watched_settlement);
					//watched_settlement.second -= timer.GetElapsedSeconds();
					//if (watched_settlement.second <= 0.0F)
					settlement_watch.timer -= timer.GetElapsedSeconds();
					if (settlement_watch.timer <= 0.0F)
					{
						// Ratios to use
						float curr_energy_ratio = settlement_energy.amount / settlement_energy.max_amount;
						float RATIO_AT_FULL = 1.0F;
						// Equalize values
						curr_energy_ratio -= ai.energy_ratio_threshold;
						RATIO_AT_FULL -= ai.energy_ratio_threshold;
						// Create linear overfill ratio
						float linear_ratio = curr_energy_ratio / RATIO_AT_FULL;
						// Increased chance calculation [Would be more efficient storing this]
						float chance_increase_at_max = ai.max_chance - ai.threshold_chance;
						// Calculate chances
						float chance_to_sprog = ai.threshold_chance + chance_increase_at_max * linear_ratio;
						// Should ensure 1.0F chance is 100%, but divisions might create odd numbers
						float contending_roll = (float)rand() / ((float)RAND_MAX + 1);

						if (chance_to_sprog > contending_roll)
						{
							std::cout << std::endl;
							std::cout << watched_settlement << " sprogged." << std::endl;
							std::cout << ">> Chance: " << chance_to_sprog << std::endl;
							std::cout << ">> Roll: " << contending_roll << std::endl;
							std::cout << std::endl;
							settlement_comp.sprogged = true;
							//cleaner.push_back(watched_settlement.first);
							coordinator->removeComponent<SettlementAIWatch>(watched_settlement);
						}
						else
						{
							std::cout << std::endl;
							std::cout << watched_settlement << " sprog failed." << std::endl;
							std::cout << ">> Chance: " << chance_to_sprog << std::endl;
							std::cout << ">> Roll: " << contending_roll << std::endl;
							settlement_watch.timer = ai.min_wait + (ai.wait_variation * (float)rand() / ((float)RAND_MAX + 1));
							std::cout << ">>>> New Wait Time: " << settlement_watch.timer << std::endl;
							std::cout << std::endl;
						}
					}
				}
			}
			/* GLORY TO HASCOMPONENT
			for (auto& removed_entity : cleaner)
			{
				auto iterator = on_watch.begin();
				while (iterator != on_watch.end())
				{
					if ((*iterator).first == removed_entity)
					{
						iterator = on_watch.erase(iterator);
					}
					else
					{
						++iterator;
					}
				}
			}
			*/
		}
	}
}

void Engine::AISprogSystem::disableAI()
{
	enabled = false;
	for (auto& entity : entities)
	{
		coordinator->removeComponent<SettlementAIWatch>(entity);
	}
}

void Engine::AISprogSystem::enableAI()
{
	enabled = true;
}

void Engine::AISprogSystem::addAIData(SprogAIData new_ai_data)
{
	ai_data.push_back(new_ai_data);
}

/*
void Engine::AISprogSystem::changeBehaviour(SprogAIData new_behaviour, const bool& clear_watches)
{
	ai_data = new_behaviour;
	
	if (clear_watches)
	{
		for (auto& entity : entities)
		{
			coordinator->removeComponent<SettlementAIWatch>(entity);
		}

		//on_watch.clear();
	}
}*/
