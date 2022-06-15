// Created by Norbert 21/03/2022

#pragma once

#include "System.h"
#include "Coordinator.h"
#include "StepTimer.h"
#include <SimpleMath.h>
#include "Particle.h"
#include "Emitter.h"

#include "RenderSystem.h"
#include "SpriteRenderer.h"
#include "Transform.h"
#include "RigidBody.h"
#include "Lifetime.h"
#include "Collider.h"
#include "Damaging.h"

using Vector3 = DirectX::SimpleMath::Vector3;
using Vector2 = DirectX::SimpleMath::Vector2;

namespace Engine
{
	class EmitterSystem : public Engine::System
	{
	public:
		EmitterSystem();
		~EmitterSystem() = default;

		void init(Coordinator* _coordinator, Engine::RenderSystem* render_sys);
		void update(const DX::StepTimer& timer);

		void createEmitter(const Emitter& emitter_str, const Vector3& velocity);
		void createFigthCloud(Vector2 mouse_pos);

	private:
		Coordinator* coordinator;
		RenderSystem* render_system;

		void createParticle(const Particle& particle_str, const Transform& transform, const RigidBody& r_body, float lifetime, std::string sprite);
	};
}



