// Created by Norbert 21/03/2022

#pragma once

#include "System.h"
#include "Coordinator.h"
#include "StepTimer.h"
#include <SimpleMath.h>

#include "Transform.h"
#include "RigidBody.h"
#include "SpriteRenderer.h"
#include "Lifetime.h"
#include "Collider.h"
#include "Damaging.h"

using Vector3 = DirectX::SimpleMath::Vector3;

namespace Engine
{
	class ParticleSystem : public Engine::System
	{
	public:
		ParticleSystem();
		~ParticleSystem() = default;

		void init(Coordinator* _coordinator);
		void update(const DX::StepTimer& timer);

	private:
		Coordinator* coordinator;
	};
}



