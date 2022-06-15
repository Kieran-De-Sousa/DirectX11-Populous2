// Created by Ben 4/4/2022

#pragma once
#include "SharedData.h"

namespace Engine
{
	/// <summary>
	/// Primarily a tag to identify a colosseum spectator
	/// </summary>
	struct ColosseumSpectator
	{
		ColosseumSpectator() = default;
		~ColosseumSpectator() = default;

		Engine::Teams team_data = Engine::Teams::RED;
	};
}
