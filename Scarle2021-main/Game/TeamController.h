#pragma once

#include "SharedData.h"

namespace Engine
{
	struct TeamController
	{
		TeamController() = default;
		~TeamController() = default;

		Teams assigned_team = Teams::BLUE;
	};
}
