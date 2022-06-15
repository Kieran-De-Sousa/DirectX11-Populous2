// Created by Norbert Kupeczki

#pragma once

#include "StepTimer.h"

#include "System.h"
#include "Coordinator.h"
#include "Hero.h"
#include "Settle.h"
#include "EnergyDecay.h"

#include "UnitSystem.h"
#include "RenderSystem.h"
#include "SpriteRenderer.h"

// Handles the creation of heroes and leader logic (eg moving towards hero)

namespace Engine
{
	/// <summary>
	/// A struct to store the information about a team in regards to having a leader,
	/// also stores the reference to the leader entity.
	/// </summary>
	struct Team {
		Engine::Teams colour = Engine::Teams::NUM_OF_TEAMS; // Just not to set the system to any of the teams by default
		Entity leader = NULL_ENTITY;

		float search_cd = 0.0F;
		float check_timer = 0.0F;
	};

	class LeaderSystem : public Engine::System
	{
	public:
		LeaderSystem();
		~LeaderSystem() = default;

		/// <summary>
		/// Initialiser function to set up the links to other systems: Unit system and Render system
		/// </summary>
		/// <param name="_coordinator pointer"></param>
		/// <param name="_unit_system pointer"></param>
		/// <param name="_render_system pointer"></param>
		void init(Coordinator* _coordinator, UnitSystem* _unit_system, RenderSystem* _render_system);

		void update(const DX::StepTimer& timer);

        /// <summary>
        /// Triggers the promotion of a leader to a hero
        /// </summary>
        /// <param name="team"></param>
        /// <param name="hero_type"></param>
        void promoteLeaderToHero(Team& team,  const HeroType hero_type);

	private:
		/// <summary>
		/// Promotes a rendom unit entity to a leader.
		/// </summary>
		/// <param name="team"></param>
		/// <param name="team_members"></param>
		void promoteUnitToLeader(Team& team, const std::vector<Entity>& team_members);

		/// <summary>
		/// Randomly selects a unit entity and calls the promoteUnitToLeader() function on it.
		/// </summary>
		/// <param name="team"></param>
		void searchNewLeader(Team& team);

		/// <summary>
		/// Updates the search timer and triggers the leader promotion event if a team
		/// has no leader for more than 10 seconds.
		/// </summary>
		/// <param name="team"></param>
		/// <param name="timer"></param>
		void updateTeamSearchTimer(Team& team, const DX::StepTimer& timer);

		/// <summary>
		/// Searches for an alive leader.
		/// </summary>
		/// <param name="team"></param>
		/// <param name="timer"></param>
		void checkLeaderIsAlive(Team& team, const DX::StepTimer& timer);

		/// <summary>
		/// A function to check for a leader in the entity list for a specific team
		/// </summary>
		/// <param name="team"></param>
		/// <returns>
		/// Returns true if the team has a leader.
		/// </returns>
		bool searchLeader(Team& team);

		Coordinator* coordinator;
		UnitSystem*  unit_system;
		RenderSystem* render_system;

		Team red_team {};
		Team blue_team {};

		const float CHECK_CD   = 2.0F;
		const float SEARCH_CD = 10.0F;
	};
} 
