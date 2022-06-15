// Created by Norbert 21/03/2022

#include "pch.h"
#include "ParticleSystem.h"
#include <iostream>

Engine::ParticleSystem::ParticleSystem() : coordinator(nullptr) {}

void Engine::ParticleSystem::init(Coordinator* _coordinator)
{
	coordinator = _coordinator;
}

void Engine::ParticleSystem::update(const DX::StepTimer& timer)
{
	for (const Entity& entity : entities)
	{
		Lifetime& lifetime = coordinator->getComponent<Lifetime>(entity);
		lifetime.lifetime -= timer.GetElapsedSeconds();
		if (lifetime.lifetime <= 0.0F)
		{
			coordinator->destroyEntityAtEndOfFrame(entity);
		}
	}
}
