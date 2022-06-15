#pragma once

#include "SharedData.h"

namespace Engine
{
	struct AbilitySpawner
	{
		AbilitySpawner() = default;
		~AbilitySpawner() = default;

		AbilityTypes ability = AbilityTypes::RAISE_LOWER_GROUND;
		Teams source_team = Teams::BLUE;
		float delay = 0.0F;
	};
}