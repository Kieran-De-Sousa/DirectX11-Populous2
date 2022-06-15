// Created by Ben 12/03/2022

#pragma once

namespace Engine
{
	struct Settle
	{
		Settle() = default;
		~Settle() = default;

		double check_placement_at = 10.0F;
		double timer = 0.0F;

		double delay_check = 3.5F;
		double delay_min = 1.0F;
		double delay_random = 5.0F;

		float min_energy = 1.0f;
	};
}