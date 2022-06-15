// Created by Ben on 21/02/2022
// Maybe this is a bad idea...

// Edit: It was

#pragma once

// System
#include <memory>

// Project 
#include "Coordinator.h"

// Allows the ECS to be accessed from anywhere (hopefully)

/* Actually I think local ecs's is better
namespace Engine
{
	class ECS
	{
	public:
		~ECS() = default;

		ECS(const ECS&) = delete;
		ECS& operator=(const ECS&) = delete;

		static Coordinator* getCoordinator()
		{
			return getInstance()->coordinator.get();
		}

	private:
		ECS()
		{
			coordinator = std::make_unique<Coordinator>();
			coordinator->init();
		}

		static ECS* getInstance()
		{
			if (instance == nullptr)
			{
				instance = new ECS();
			}
			return instance;
		}

		static ECS* instance;

		std::unique_ptr<Coordinator> coordinator;
	};
}
*/

// Header only because the rest of the ECS system is header only (because template magic) so
// thought we might as well stay consistent... 
// But if this one file is causing m a s s i v e complication slowdown we can change it 
