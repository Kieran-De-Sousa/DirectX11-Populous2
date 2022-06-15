#include "pch.h"

#include "Leader.h"
#include "Unit.h"
#include "MoreMath.h"
#include "Driver.h"

#include "LeaderSystem.h"

Engine::LeaderSystem::LeaderSystem() : coordinator(nullptr), unit_system(nullptr)
{
	// Can't initialise variables in here because of header only circular dependency issues
}

void Engine::LeaderSystem::init(Coordinator* _coordinator, UnitSystem* _unit_system, RenderSystem* _render_system)
{
	coordinator = _coordinator;
	unit_system = _unit_system;
	render_system = _render_system;

	red_team.colour = Engine::Teams::RED;
	blue_team.colour = Engine::Teams::BLUE;
}

void Engine::LeaderSystem::update(const DX::StepTimer& timer)
{
	checkLeaderIsAlive(red_team, timer);              // First we check whether the teams have leaders
	checkLeaderIsAlive(blue_team, timer);

	updateTeamSearchTimer(red_team, timer);  // If any of the leader doesn't exist, update the timer and appoint a new when the timer reaches 0.
	updateTeamSearchTimer(blue_team, timer);
}

// Appoints a new leader from the population at random

void Engine::LeaderSystem::promoteUnitToLeader(Team& team, const std::vector<Entity>& team_members)
{
	team.leader = team_members[HelperMethods::MoreMath::randomRange(0, team_members.size() - 1)]; // Store the new leaders entity ID

	coordinator->addComponent<Engine::Leader>(team.leader, {});																		   // Add a component to the entity to tag it
	coordinator->removeComponent<EnergyDecay>(team.leader);
	coordinator->removeComponent<Settle>(team.leader);

	Entity crowning_the_king = coordinator->createChild(team.leader);
	auto& crown_transform = coordinator->addComponent<Transform>(crowning_the_king, {});
	crown_transform.position = coordinator->getComponent<Transform>(team.leader).position;
	crown_transform.follow_parent = true;
	auto& crown_sprite = coordinator->addComponent<SpriteRenderer>(crowning_the_king, {});
	render_system->initSprite(Engine::Driver::getLoadHandler()->getResourcePath(Engine::LoadHandler::Resources::HOUSE_TEXTURE_B), crowning_the_king);
}

// Promotes the leader of a team to hero

void Engine::LeaderSystem::promoteLeaderToHero(Team& team, const HeroType hero_type)
{
	Unit& leader_unit_component = coordinator->getComponent<Unit>(team.leader);

	team.leader = NULL_ENTITY;
	coordinator->removeComponent<Leader>(team.leader);
	Hero& hero_comp = coordinator->addComponent<Hero>(team.leader, {});
	hero_comp.hero_type = hero_type;
}

// Searches a new leader by listing all the entities that belongs to a specific team
// Once the function got all the team members, it calls the relevant function to randomly select
// a member to become the new leader. 

void Engine::LeaderSystem::searchNewLeader(Team& team)
{	
	std::vector<Entity> team_members;
	for (const Entity& entity : entities)
	{
		Unit& unit = coordinator->getComponent<Unit>(entity);
		if (unit.unit_team == team.colour)
		{
			team_members.emplace_back(entity);	
		}		
	}
	promoteUnitToLeader(team, team_members);
}

// If a team doesn't have a leader, after 10 seconds a new leader is appointed.
// This function updates the search timer of a team.

void Engine::LeaderSystem::updateTeamSearchTimer(Team& team, const DX::StepTimer& timer)
{
	if (team.leader == NULL_ENTITY)        // If the leader doesn't exist...
	{
		if (team.search_cd <= 0.0F)              // ... and the timer reached 0 (or less) ...
		{
			searchNewLeader(team);			   // ... a new leader is then selected.
			team.search_cd = SEARCH_CD;  // Then the timer is reset.
		}
		else														   // If there is no leader, but the timer didn't reach zero yet, keep the timer ticking.
		{
			team.search_cd -= timer.GetElapsedSeconds();
		}
	}
}

// At every 2 seconds this function updates the state of the leader

void Engine::LeaderSystem::checkLeaderIsAlive(Team& team, const DX::StepTimer& timer)
{
	if (team.check_timer <= 0.0F)			   // Once the timer reaches 0 (or less) ...
	{
		if (searchLeader(team))			           // ... we check whether the leader is still alive or not ...
		{
			team.check_timer = CHECK_CD; //... resetting the timer and exiting the function if it sill is.
			return;
		}
		
		team.leader = NULL_ENTITY;           // If the leader is dead, set it to NULL_ENTITY, and reset the timers.
		team.search_cd = SEARCH_CD;
		team.check_timer = CHECK_CD;
	}
	else                                                            // Otherwise just keep the timer ticking
	{
		team.check_timer -= timer.GetElapsedSeconds();
	}
}

// Searching for the leader amongst the team members

bool Engine::LeaderSystem::searchLeader(Team& team)
{
	for (const Entity& entity : entities)
	{
		Unit& unit = coordinator->getComponent<Unit>(entity);
		if (coordinator->hasComponent<Leader>(entity) && unit.unit_team == team.colour)  // returning true if a leader with a matching colour attribute
		{
			return true;
		}
	}

	return false;
}