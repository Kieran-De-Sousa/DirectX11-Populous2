#pragma once

#include "Types.h"
#include <SimpleMath.h>

namespace Engine
{
	struct EnergyFlagComponent
	{
		EnergyFlagComponent() = default;
		~EnergyFlagComponent() = default;

		float energy_ratio = 0.0F;
		float lift_height = 32.0F;
		
		DirectX::SimpleMath::Vector3 offset;
		
		Entity building = NULL_ENTITY;
	};
}