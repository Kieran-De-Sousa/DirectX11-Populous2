#pragma once

#include "SharedData.h"

// Simon said this project should be 'an engine for this type of game', 
// so does that mean units are generic enough to be classified as 'Engine'?

namespace Engine
{
	/// <summary>
	/// Base component that all units have
	/// </summary>
	struct Unit
	{
		Unit() = default;
		~Unit() = default;

		Engine::Teams unit_team = Engine::Teams::RED;

		enum class UnitType {
			Normal
			// Any other unit types here
		};
		UnitType unit_type;

		float energy = 10.0F;
		float max_energy = 10.0F;

		double action_delay = 3.0; // How long after an action (eg fighting, merging, etc) the unit has to wait before it can do another action
		double action_clock = 0.0;
	};
}
