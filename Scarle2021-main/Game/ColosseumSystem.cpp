// Created by Ben 04/04/2022

#include "pch.h"

#include "ColosseumSpectator.h"
#include "Transform.h"

#include "ColosseumSystem.h"

Engine::ColosseumSystem::ColosseumSystem() : coordinator(nullptr)
{
	// Can't initialise variables in here because of header only circular dependency issues
}

void Engine::ColosseumSystem::init(Coordinator* _coordinator, const DirectX::SimpleMath::Vector3& _position, unsigned int _col_width, unsigned int _col_height, float _seat_width, float _seat_height)
{
	coordinator = _coordinator;

	position    = _position;
	col_width   = _col_width;
	col_height  = _col_height;
	seat_width  = _seat_width;
	seat_height = _seat_height;
}

void Engine::ColosseumSystem::update(DX::StepTimer const& timer)
{  
	unsigned int blue_team_seat_x = 0;
	unsigned int blue_team_seat_y = 0;

	unsigned int red_team_seat_x  = col_width;
	unsigned int red_team_seat_y  = 0;

	DirectX::SimpleMath::Vector3 seat_pos{};

	for (const Entity& entity : entities)
	{
		Transform& transform         = coordinator->getComponent<Engine::Transform>(entity);
		ColosseumSpectator& spectator = coordinator->getComponent<Engine::ColosseumSpectator>(entity);

		if (spectator.team_data == Engine::Teams::BLUE)
		{
			seat_pos.x = seat_width  * blue_team_seat_x;
			seat_pos.y = seat_height * blue_team_seat_y;

			blue_team_seat_y++;
			if (blue_team_seat_y == col_height)
			{
				blue_team_seat_x++;
				blue_team_seat_y = 0;
			}
		}
		else
		{
			seat_pos.x = seat_width  * red_team_seat_x;
			seat_pos.y = seat_height * red_team_seat_y;

			red_team_seat_y++;
			if (red_team_seat_y == col_height)
			{
				red_team_seat_x--;
				red_team_seat_y = 0;
			}
		}

		transform.position = position + seat_pos;
	}
}
