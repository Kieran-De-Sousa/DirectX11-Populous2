#pragma once
////////! Systems !////////
/// Rendering ///
#include "CameraSystem.h"
#include "RenderSystem.h"
#include "TextSystem.h"
#include "AutoScaleSystem.h"
#include "UiManager.h"

/// Bodies ///
#include "CollisionSystem.h"
#include "PhysicsSystem.h"

/// Unit ///
#include "UnitSystem.h"
#include "LeaderSystem.h"

/// Settlement ///
#include "SettleSystem.h"
#include "SettlementSpawnerSystem.h"

/// AI ///
#include "PathFinder.h"
#include "AISettlementWatcherSystem.h"
#include "AISprogSystem.h"
#include "AbilityAISystem.h"
#include "AIAbilitySystem.h"
#include "FlagCreationSystem.h"
#include "FlagRaisingSystem.h"
#include "BoundarySystem.h"
#include "WorshipSystem.h"

/// Abilities ///
#include "AbilityManager.h"
#include "ParticleSystem.h"
#include "EmitterSystem.h"
#include "DecaySystem.h"
#include "EnergyRegenSystem.h"

/// ECS ///
#include "Driver.h"
#include "Coordinator.h"
#include "ColosseumSystem.h"
#include "BackgroundCloudSystem.h"

class SceneGame : public Engine::Scene
{
public:
	SceneGame(const GameScenes& scene_type);
	SceneGame() = default;
	~SceneGame() override = default;

	//-----------------------------------! Virtual Functions Inherrited From Scene !-----------------------------------//
	virtual void init();
	virtual void initComponents();
	virtual void initSystems();
	virtual void initAudio();
	virtual void update(DX::StepTimer const& timer);
	virtual void render(DX::StepTimer const& timer);
	virtual void onKey(const DirectX::Keyboard::KeyboardStateTracker& keyboard_data);
	virtual void onMouse(const DirectX::Mouse::ButtonStateTracker& mouse_data);
	virtual void setPause(const bool& pause) override;
	//------------------------------------------------------------------------------------------------------------------//

private:

	//------------------------------------------------------! ECS !-----------------------------------------------------//
	std::unique_ptr<Engine::Coordinator> ecs;
	//------------------------------------------------------------------------------------------------------------------//


	//---------------------------------------------------! Systems !----------------------------------------------------//
		    //-------------------------------------------- Rendering --------------------------------------------//
	std::shared_ptr<Engine::CameraSystem> camera_system												= nullptr;
	std::shared_ptr<Engine::RenderSystem> render_system													= nullptr;
	std::shared_ptr<Engine::TextSystem> text_system																= nullptr;
	std::shared_ptr<Engine::AutoScaleSystem> scale_system												= nullptr;
	std::shared_ptr<Engine::UiManager> ui_canvas																	= nullptr;
	std::shared_ptr<Engine::ColosseumSystem> col_system													= nullptr;
	std::shared_ptr<Engine::BackgroundCloudSystem> cloud_system									= nullptr;
			//-------------------------------------------- Bodies -----------------------------------------------//
	std::shared_ptr<Engine::PhysicsSystem> physics_system												= nullptr;
	std::shared_ptr<Engine::CollisionSystem> collision_system												= nullptr;
			//------------------------------------------- Tile Map ----------------------------------------------//
	std::unique_ptr<Engine::TileMap> tile_map																			= nullptr;
			//------------------------------------------- Unit --------------------------------------------------//
	std::shared_ptr<Engine::UnitSystem> unit_system																= nullptr;
	std::shared_ptr<Engine::LeaderSystem> leader_system													= nullptr;
		    //------------------------------------------- Settlement --------------------------------------------//
	std::shared_ptr<Engine::SettleSystem> settle_system														= nullptr;
			//--------------------------------------------- AI --------------------------------------------------//
	std::shared_ptr<Engine::SettlementSpawnerSystem> settlement_spawner_system	= nullptr;
	std::unique_ptr<Engine::PathFinder> path_finder																= nullptr;
	std::shared_ptr<Engine::FlagCreationSystem> flag_creation_system								= nullptr;
	std::shared_ptr<Engine::FlagRaisingSystem> flag_raising_system									= nullptr;
	std::shared_ptr<Engine::AISprogSystem> ai_sprog_system												= nullptr;
	std::shared_ptr<Engine::AISettlementWatchSystem> ai_settlement_watch_system = nullptr;
	std::shared_ptr<Engine::BoundarySystem> boundary_system										= nullptr;
	std::shared_ptr<Engine::AbilityAISystem> ability_ai_system												= nullptr;
	std::shared_ptr<Engine::AIAbilitySystem> ai_ability_system												= nullptr;
		    //------------------------------------------- Abilities --------------------------------------------//
	std::unique_ptr<AbilityManager> ability_manager																= nullptr;
	std::shared_ptr<Engine::EmitterSystem> emitter_system													= nullptr;
	std::shared_ptr<Engine::ParticleSystem> particle_system												= nullptr;
	std::shared_ptr<Engine::DecaySystem> decay_system														= nullptr;
	std::shared_ptr<Engine::EnergyRegenSystem> energy_regen_system							= nullptr;
	std::shared_ptr<Engine::WorshipSystem> worship_system												= nullptr;
	//----------------------------------------------------------------------------------------------------------------//
	
	//---------------------------------------------------! Game State !-------------------------------------------------//
			//--------------------------------------------- Inputs ---------------------------------------------//
	void inputLeftClick();
	void inputRightClick();
			//--------------------------------------------- Entities ---------------------------------------------//
	Engine::Entity main_camera;
	void createUnits(unsigned int number_of_units);
			//-------------------------------------------- Tile Map ---------------------------------------------//
	const std::map<Engine::TileMap::Tile::TileState, std::string> tile_textures
	{
		{ Engine::TileMap::Tile::TileState::Grass, "GrassFlat" },
		{ Engine::TileMap::Tile::TileState::Water, "WaterFlat" },
		{ Engine::TileMap::Tile::TileState::Mountain1, "GrassHeight1" },
		{ Engine::TileMap::Tile::TileState::Mountain2, "GrassHeight2" },
		{ Engine::TileMap::Tile::TileState::Mountain3, "GrassHeight3" } 
	};
	const DirectX::SimpleMath::Vector2 GRID_SIZE{ 32, 32 };
	const DirectX::SimpleMath::Vector2 TILE_SIZE{ 32, 16 };
			//------------------------------------------- Abilities --------------------------------------------//
	void addAbilityFireStorm(std::shared_ptr<Engine::AbilityAISystem> target_ai);
	void addAbilityPillarOfFire(std::shared_ptr<Engine::AbilityAISystem> target_ai);
	//------------------------------------------------------------------------------------------------------------------//

	void onCollisionEnterTest(const Engine::Entity& t, const Engine::Entity& v);
	void onCollisionExitTest(const Engine::Entity& t, const Engine::Entity& v);
	int test = 0; /// The coconut

	/// Arcade machine input variables
	short x_input = 0.0F;
	short y_input = 0.0F;
	unsigned short  booster = 1.0F;

	/// <summary>
	/// Button press sound 
	/// </summary>
	Engine::AudioHandler::SoundData button_press_sound{};
	/// <summary>
	/// Pillar of fire sound
	/// </summary>
	Engine::AudioHandler::SoundData pillar_of_fire_sound{};
	/// <summary>
	/// NPC Death noise sound
	/// </summary>
	Engine::AudioHandler::SoundData npc_death_sound{};
	Engine::Entity team_blue_controller;
};