#include "pch.h"

#include "SettlementSpawnerSystem.h"
#include "Driver.h"

#include <iostream>

Engine::SettlementSpawnerSystem::SettlementSpawnerSystem() : coordinator(nullptr)
{
	// Can't initialise coordinator in here because of header only circular dependency issues
}

void Engine::SettlementSpawnerSystem::init(Coordinator* _coordinator, UnitSystem* _unit_system, const std::function<void(const Engine::Entity&, const Engine::Entity&)>& _on_collision_enter, const std::function<void(const Engine::Entity&, const Engine::Entity&)>& _on_collision_exit)
{
	coordinator = _coordinator;
	unit_system = _unit_system;
	on_collision_func = _on_collision_enter;
	off_collision_func = _on_collision_exit;
}

void Engine::SettlementSpawnerSystem::update(const DX::StepTimer& timer)
{
	for (const auto& entity : entities)
	{
		Settlement& settlement_comp = coordinator->getComponent<Settlement>(entity);
		Transform& settlement_transform = coordinator->getComponent<Transform>(entity);
		Energy& settlement_energy = coordinator->getComponent<Energy>(entity);
		if (settlement_comp.sprogged && settlement_energy.amount > settlement_energy.min_amount)
		{
			settlement_comp.sprogged = false;
			//Spawn a unit
			Entity new_unit = coordinator->createEntity();
			Transform unit_transform;
			RigidBody unit_rigid;
			Unit unit_data;
			//TODO: FIX THIS
			DirectX::SimpleMath::Vector3 temp_fix = { 8,8,0 };
			unit_transform.position = settlement_transform.position + temp_fix;
			unit_data.energy = settlement_energy.amount;
			unit_data.unit_team = settlement_comp.building_team;
			settlement_energy.amount = 0.0F;
			//Currently using the unit system to do this, wish this was decoupled 
			unit_system->initAgent(
				new_unit,
				unit_transform,
				unit_rigid,
				unit_data,
				Engine::Driver::getLoadHandler()->getResourcePath(unit_data.unit_team == Engine::Teams::RED ? Engine::LoadHandler::Resources::CHARACTER_TEXTURE :
					Engine::LoadHandler::Resources::ENEMY_TEXTURE), 
				on_collision_func,
				off_collision_func,
				{ 10,10 },
				200, 20);
		}
	}
}