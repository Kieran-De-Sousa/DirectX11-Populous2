#pragma once

#include <memory>

namespace Engine
{
	struct ManaPool
	{
		ManaPool() { mana_pool = std::make_unique<float>(0.0F); }
		ManaPool(float init_value) { mana_pool = std::make_unique<float>(init_value); }
		~ManaPool() = default;

		std::shared_ptr<float> mana_pool = nullptr;
	};
}
