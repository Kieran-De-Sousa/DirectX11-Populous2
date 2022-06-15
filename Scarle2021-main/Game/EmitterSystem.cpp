// Created by Norbert 21/03/2022

#include "pch.h"
#include "EmitterSystem.h"
#include <iostream>

Engine::EmitterSystem::EmitterSystem() : coordinator(nullptr), render_system(nullptr) {}

void Engine::EmitterSystem::init(Coordinator* _coordinator, Engine::RenderSystem* render_sys)
{
	//initializes the system
	coordinator = _coordinator;
	render_system = render_sys;
}

void Engine::EmitterSystem::update(const DX::StepTimer& timer)
{
	for (const Entity& entity : entities)
	{
		Lifetime& lifetime = coordinator->getComponent<Lifetime>(entity);
		lifetime.lifetime -= timer.GetElapsedSeconds();

		if (lifetime.lifetime <= 0.0F)
		{
			coordinator->destroyEntityAtEndOfFrame(entity);
		}
		else
		{
			Emitter& emitter = coordinator->getComponent<Emitter>(entity);
			Transform& transform = coordinator->getComponent<Transform>(entity);
			RigidBody& emitter_r_body = coordinator->getComponent<RigidBody>(entity);
			emitter.emission_cd -= timer.GetElapsedSeconds();

			if (emitter.emission_cd <= 0.0F)
			{
				ParticleTypes particle_type = ParticleTypes::UNDEFINED;
				switch (emitter.particle_type)
				{
				case EmitterTypes::HORIZONTAL:
				{
					particle_type = ParticleTypes::HORIZONTAL;
				} break;
				case EmitterTypes::VERTICAL:
				{
					particle_type = ParticleTypes::VERTICAL;
				} break;
				case EmitterTypes::TILE:
				{
					particle_type = ParticleTypes::TILE;
				} break;
				default:
				{
					particle_type = ParticleTypes::UNDEFINED;
				}
				break;
				}
				Particle particle{ particle_type };

				float x_spd = 0.0F;
				float y_spd = 0.0F;

				float x_transf = 0.0F;
				float y_transf = 0.0F;

				if (particle.particle_type == ParticleTypes::HORIZONTAL)
				{
					x_spd = ((float)rand() / ((float)RAND_MAX + 1) - 0.5F) * emitter.particle_speed;
					y_spd = ((float)rand() / ((float)RAND_MAX + 1) - 0.5F) * emitter.particle_speed;
				}
				else if (particle.particle_type == ParticleTypes::VERTICAL)
				{
					x_spd = ((float)rand() / ((float)RAND_MAX + 1) - 0.5F) * 10;
					y_spd = ((float)rand() / ((float)RAND_MAX + 1) - 1.0F) * emitter.particle_speed;
				}
				else if (particle.particle_type == ParticleTypes::TILE)
				{
					x_transf = ((float)rand() / ((float)RAND_MAX + 1) - 0.5F) * emitter.tile_spread;
					y_transf = ((float)rand() / ((float)RAND_MAX + 1) - 0.5F) * emitter.tile_spread;
				}

				if (emitter.damage_type == Engine::DamageType::DAMAGING_PARTICLE ||
					emitter.damage_type == Engine::DamageType::DAMAGING_BOTH)
				{
					particle.damaging = true;
					particle.damage   = emitter.damage;

				}

				RigidBody r_body{ Vector3(x_spd, y_spd, 0), Vector3(0, 0, 0), 0 };
				if (!emitter.world_space_particles)
				{
					r_body.velocity += emitter_r_body.velocity;
					r_body.acceleration += emitter_r_body.acceleration;
				}

				Transform spawn_pos{};
				spawn_pos.position.x = transform.position.x + x_transf;
				spawn_pos.position.y = transform.position.y + y_transf;
				spawn_pos.position.z = transform.position.z;

				emitter.emission_cd = 1.0F / float(emitter.emission_speed);
				createParticle(particle, spawn_pos, r_body, emitter.particle_lifetime, emitter.sprite);
							
			}		
		}
	}
}

void Engine::EmitterSystem::createEmitter(const Emitter& emitter_str, const Vector3& velocity)
{
	//Transform transform { Vector3(emitter_str.location.x, emitter_str.location.y, 0) };
	Engine::Entity emitter = coordinator->createEntity();

	Emitter& emitter_comp = coordinator->addComponent<Emitter>(emitter, {});
	emitter_comp = emitter_str;

	Transform& transform2 = coordinator->addComponent<Transform>(emitter, {});
	transform2.position = Vector3(emitter_str.location.x, emitter_str.location.y, 0);

	Lifetime& lifetime_comp = coordinator->addComponent<Lifetime>(emitter, {});
	lifetime_comp.lifetime = emitter_str.emitter_lifetime;

	RigidBody& r_body_comp = coordinator->addComponent<Engine::RigidBody>(emitter, {});
	r_body_comp.gravity_scale = 1;
	r_body_comp.velocity = velocity;
	r_body_comp.acceleration = Vector3(0, 0, 0);

	Collider& collider_comp = coordinator->addComponent<Engine::Collider>(emitter, {});
	collider_comp.collider_size = {32.0F,32.0F};

	if (emitter_str.damage_type == Engine::DamageType::DAMAGING_EMITTER ||
		emitter_str.damage_type == Engine::DamageType::DAMAGING_BOTH)
	{
		DamagingAbility& damaging_comp = coordinator->addComponent<Engine::DamagingAbility>(emitter, {});
		emitter_comp.damage = emitter_str.damage;
		emitter_comp.damage_type = emitter_str.damage_type;
	}
}

void Engine::EmitterSystem::createFigthCloud(Vector2 mouse_pos)
{
	Engine::Emitter emitter{};
	emitter.particle_type = Engine::EmitterTypes::HORIZONTAL;
	emitter.tile_spread = 10.0F;
	emitter.location = mouse_pos;
	emitter.emission_speed = 5;
	emitter.sprite = "cloud";
	emitter.particle_lifetime = 1.0F;
	emitter.particle_speed = 20.0F;
	emitter.emitter_lifetime = 0.5F;

	Vector3 velocity(0, 0, 0);
	
	createEmitter(emitter, {0.0F,0.0F,0.0F});
}

void Engine::EmitterSystem::createParticle(const Particle& particle_str, const Transform& transform, const RigidBody& r_body, float lifetime, std::string sprite)
{
	Engine::Entity particle = coordinator->createEntity();
	coordinator->addComponent<Engine::Particle>(particle, particle_str);
	coordinator->addComponent<Engine::Transform>(particle, transform);
	coordinator->addComponent<Engine::RigidBody>(particle, r_body);
	coordinator->addComponent<Engine::SpriteRenderer>(particle, {});
	coordinator->addComponent<Engine::Lifetime>(particle, {lifetime});

	if (particle_str.damaging)
	{
		coordinator->addComponent<Engine::DamagingAbility>(particle, {});
	}

	Collider& collider_comp = coordinator->addComponent<Engine::Collider>(particle, {});
	collider_comp.collider_size = { 32.0F,32.0F };

	render_system->initSprite(sprite, particle);
}