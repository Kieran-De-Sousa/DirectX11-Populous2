#pragma once

#include "pch.h"
#include "SharedData.h"
#include "Types.h"


namespace Engine
{
	struct Settlement
	{
		Settlement() = default;
		~Settlement() = default;

		Engine::Teams building_team = Engine::Teams::RED;
		Engine::BuildingTypes settlement_type = Engine::BuildingTypes::HUT;
		float energy_per_sec = 0.5F;

		Entity flag_entity = NULL_ENTITY;

		bool sprogged = false;
	};
}