// Created by Norbert on 21/03/2022

#pragma once

// A component to handle the lifetime of any entity
// Holds only the lifetime, what happens with the entity
// having this component depends on the logic that handles
// that specific entity.

namespace Engine
{
	struct Lifetime
	{
		Lifetime() = default;
		~Lifetime() = default;

		float lifetime = 0.0F;
	};
}