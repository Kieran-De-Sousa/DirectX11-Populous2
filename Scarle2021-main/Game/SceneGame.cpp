#include "pch.h"

////////! Components !////////
/// Rendering ///
#include "SpriteRenderer.h"
#include "UI.h"

/// Positioning & Bodies ///
#include "Transform.h"
#include "Collider.h"
#include "RigidBody.h"

/// Unit ///
#include "PathFinderAgent.h"
#include "Unit.h"
#include "Leader.h"

/// Settlement ///
#include "Settle.h"
#include "Settlement.h"
#include "SettlementAIWatch.h"

/// Ability ///
#include "Energy.h"
#include "EnergyDecay.h"
#include "EnergyFlagComponent.h"
#include "Particle.h"
#include "Emitter.h"

////! Needed to complile !////
#include <SimpleMath.h>

////! Header !////
#include "SceneGame.h"
#include "ColosseumSpectator.h"
#include "ColosseumStadium.h"
#include "Cloud.h"


SceneGame::SceneGame(const GameScenes& scene_type) : Scene(scene_type) {}

void SceneGame::init()
{
	/// Reset scene data struct 
	setDefaultSceneData();

	/// Instantiate ECS
	ecs = std::make_unique<Engine::Coordinator>();
	ecs->init();

	initComponents();
	initSystems();
}

void SceneGame::initComponents()
{
	/// Rendering ///
	ecs->registerComponent<Engine::SpriteRenderer>();
	ecs->registerComponent<Engine::TextRenderer>();
	ecs->registerComponent<Engine::UI>();
	ecs->registerComponent<Engine::UIButton>();
	ecs->registerComponent<Engine::UIButtonGroup>();
	ecs->registerComponent<Engine::UIButtonAbility>();
	ecs->registerComponent<Engine::Camera>();
	ecs->registerComponent<Engine::Cloud>();
	ecs->registerComponent<Engine::ColosseumSpectator>();
	ecs->registerComponent<Engine::ColosseumStadium>();

	/// Positioning & Bodies ///
	ecs->registerComponent<Engine::Transform>();
	ecs->registerComponent<Engine::Collider>();
	ecs->registerComponent<Engine::RigidBody>();

	/// Unit ///
	ecs->registerComponent<Engine::PathFinderAgent>();
	ecs->registerComponent<Engine::Unit>();
	ecs->registerComponent<Engine::Hero>();
	ecs->registerComponent<Engine::Leader>();
	ecs->registerComponent<Engine::TeamController>();

	/// Settlement ///
	ecs->registerComponent<Engine::Settle>();
	ecs->registerComponent<Engine::Settlement>();
	ecs->registerComponent<Engine::SettlementAIWatch>();

	/// Ability ///
	ecs->registerComponent<Engine::Energy>();
	ecs->registerComponent<Engine::EnergyFlagComponent>();
	ecs->registerComponent<Engine::EnergyDecay>();
	ecs->registerComponent<Engine::DamagingAbility>();
	ecs->registerComponent<Engine::AbilitySpawner>();
	ecs->registerComponent<Engine::ManaPool>();
	ecs->registerComponent<Engine::Particle>();
	ecs->registerComponent<Engine::Emitter>();
	ecs->registerComponent<Engine::Lifetime>();
}

void SceneGame::initSystems()
{
	////! Ability System !////
	ability_manager = std::make_unique<AbilityManager>();

#pragma region Rendering
	/// Get current window information
	const DirectX::SimpleMath::Vector2 window_size = Engine::Driver::getDriver()->getWindowSize();
	const float aspect_ratio = window_size.x / window_size.y;

	////! Camera !////
	camera_system = ecs->registerSystem<Engine::CameraSystem>();
	{
		Engine::Signature signature;
		signature.set(ecs->getComponentType<Engine::Transform>());
		signature.set(ecs->getComponentType<Engine::Camera>());
		ecs->setSystemSignature<Engine::CameraSystem>(signature);
	}
	camera_system->init(ecs.get());
	
	////! Camera Entity !////
	/// Create main camera component
	main_camera = ecs->createEntity();
	ecs->addComponent<Engine::Transform>(main_camera,
		{
			DirectX::SimpleMath::Vector3(0.0f, 200.0f, 200.0f),
			DirectX::SimpleMath::Vector2::One, 0
		});
	ecs->addComponent<Engine::Camera>(main_camera, {}).makeProjectionTransform(0.25f * XM_PI, aspect_ratio, 1.0f, 10000.0f,
		DirectX::SimpleMath::Vector3::UnitY, DirectX::SimpleMath::Vector3::Zero);
	
	/// Assign camera component to DirectX camera
	Engine::Driver::getDriver()->createDrawData(
		&ecs->getComponent<Engine::Camera>(main_camera), nullptr);

	////! Sprite Render System !////
	render_system = ecs->registerSystem<Engine::RenderSystem>();
	{
		Engine::Signature signature;
		signature.set(ecs->getComponentType<Engine::Transform>());
		signature.set(ecs->getComponentType<Engine::SpriteRenderer>());
		ecs->setSystemSignature<Engine::RenderSystem>(signature);
	}
	render_system->init(ecs.get(), main_camera);

	////! Text Render System !////
	text_system = ecs->registerSystem<Engine::TextSystem>();
	{
		Engine::Signature signature;
		signature.set(ecs->getComponentType<Engine::Transform>());
		signature.set(ecs->getComponentType<Engine::TextRenderer>());
		ecs->setSystemSignature<Engine::TextSystem>(signature);
	}
	text_system->init(ecs.get());

	////! Scaling Render System !////
	scale_system = ecs->registerSystem<Engine::AutoScaleSystem>();
	{
		Engine::Signature signature;
		signature.set(ecs->getComponentType<Engine::Transform>());
		ecs->setSystemSignature<Engine::AutoScaleSystem>(signature);
	}
	scale_system->init(ecs.get());

	// Cloud system
	cloud_system = ecs->registerSystem<Engine::BackgroundCloudSystem>();
	{
		Engine::Signature signature;
		signature.set(ecs->getComponentType<Engine::Cloud>());
		signature.set(ecs->getComponentType<Engine::Transform>());
		signature.set(ecs->getComponentType<Engine::SpriteRenderer>());

		ecs->setSystemSignature<Engine::BackgroundCloudSystem>(signature);
	}
	cloud_system->init(ecs.get(), Engine::Driver::getDriver()->getWindowSize(), 100);

	Engine::BackgroundCloudSystem::CloudCreationData cloud_settings{};
	cloud_settings.cloud_images = {
		"../Assets/cloud_background_1", "../Assets/cloud_background_2"
	};
	cloud_settings.cloud_spacing = 810;
	cloud_system->createCloud(render_system, cloud_settings);

	// Colosseum system
	col_system = ecs->registerSystem<Engine::ColosseumSystem>();
	{
		Engine::Signature signature;
		signature.set(ecs->getComponentType<Engine::Transform>());
		signature.set(ecs->getComponentType<Engine::ColosseumSpectator>());

		ecs->setSystemSignature<Engine::ColosseumSystem>(signature);
	}
	col_system->init(ecs.get(), { 30, 120, 0 }, 17, 9, 20, 20);

	Engine::Entity col_background = ecs->createEntity();
	ecs->addComponent<Engine::Transform>(
		col_background,
		{ DirectX::SimpleMath::Vector3(200, 200, 0), {2, 3}, 0 });
	ecs->addComponent<Engine::SpriteRenderer>(
		col_background,
		{}).move_with_camera = false;
	ecs->addComponent<Engine::ColosseumStadium>(col_background, {});

	render_system->initSprite("../Assets/colloseum", col_background);
#pragma endregion

#pragma region Bodies
	////! Physics System !////
	physics_system =
		ecs->registerSystem<Engine::PhysicsSystem>();
	{
		// The components an entity will have to have to be a part of this system
		Engine::Signature signature;
		signature.set(ecs->getComponentType<Engine::Transform>());
		signature.set(ecs->getComponentType<Engine::RigidBody>());
		ecs->setSystemSignature<Engine::PhysicsSystem>(signature);
	}
	physics_system->init(ecs.get());

	////! Collision System !////
	collision_system = ecs->registerSystem<Engine::CollisionSystem>();
	{
		Engine::Signature signature;
		signature.set(ecs->getComponentType<Engine::Collider>());
		ecs->setSystemSignature<Engine::CollisionSystem>(signature);
	}
	collision_system->init(ecs.get());
#pragma endregion

#pragma region Tilemap
	////! Tilemap !////
	tile_map = std::make_unique<Engine::TileMap>();
	tile_map->generateFromRandom(GRID_SIZE.x, GRID_SIZE.y);
	tile_map->drawMap(ecs.get(), render_system.get(), tile_textures, TILE_SIZE, DirectX::SimpleMath::Vector2::One, 0,
		{ window_size.x / 2, (window_size.y / 2) - (GRID_SIZE.y * TILE_SIZE.y / 2) });
#pragma endregion

#pragma region AI
	////! Flag Creation System !////
	flag_creation_system = ecs->registerSystem<Engine::FlagCreationSystem>();
	{
		Engine::Signature signature;
		signature.set(ecs->getComponentType<Engine::Transform>());
		signature.set(ecs->getComponentType<Engine::Energy>());
		signature.set(ecs->getComponentType<Engine::Settlement>());

		ecs->setSystemSignature<Engine::FlagCreationSystem>(signature);
	}
	flag_creation_system->init(ecs.get(), render_system.get());

	////! Flag Raising System !////
	flag_raising_system = ecs->registerSystem<Engine::FlagRaisingSystem>();
	{
		Engine::Signature signature;
		signature.set(ecs->getComponentType<Engine::Transform>());
		signature.set(ecs->getComponentType<Engine::EnergyFlagComponent>());

		ecs->setSystemSignature<Engine::FlagRaisingSystem>(signature);
	}
	flag_raising_system->init(ecs.get());

	////! AI Sprog System !////
	/// Create AI data structs
	Engine::SprogAIData player_ai_data;
	Engine::SprogAIData enemy_ai_data;
	enemy_ai_data.ai_team = Engine::Teams::RED;
	player_ai_data.ai_team = Engine::Teams::BLUE;

	ai_settlement_watch_system = ecs->registerSystem<Engine::AISettlementWatchSystem>();
	{
		Engine::Signature signature;
		signature.set(ecs->getComponentType<Engine::Settlement>());
		signature.set(ecs->getComponentType<Engine::Energy>());

		ecs->setSystemSignature<Engine::AISettlementWatchSystem>(signature);
	}
	ai_settlement_watch_system->init(ecs.get());
	ai_settlement_watch_system->addTeamAI(player_ai_data);
	ai_settlement_watch_system->addTeamAI(enemy_ai_data);
	
	ai_sprog_system = ecs->registerSystem<Engine::AISprogSystem>();
	{
		Engine::Signature signature;
		//signature.set(ecs->getComponentType<Engine::SettlementAIWatch>()); Invalidates for loop atm
		signature.set(ecs->getComponentType<Engine::Settlement>());
		signature.set(ecs->getComponentType<Engine::Energy>());

		ecs->setSystemSignature<Engine::AISprogSystem>(signature);
	}
	ai_sprog_system->init(ecs.get());
	ai_sprog_system->addAIData(player_ai_data);
	ai_sprog_system->addAIData(enemy_ai_data);

	////! Boundary System !////
	boundary_system = ecs->registerSystem<Engine::BoundarySystem>();
	{
		Engine::Signature signature;
		signature.set(ecs->getComponentType<Engine::Unit>());
		signature.set(ecs->getComponentType<Engine::PathFinderAgent>());
		signature.set(ecs->getComponentType<Engine::Transform>());

		ecs->setSystemSignature<Engine::BoundarySystem>(signature);
	}
	boundary_system->init(ecs.get(), tile_map.get());

	
	//---------------------- THE Leader system ----------------------

	leader_system = ecs->registerSystem<Engine::LeaderSystem>();
	{
		Engine::Signature signature;
		signature.set(ecs->getComponentType<Engine::Transform>());
		signature.set(ecs->getComponentType<Engine::RigidBody>());
		signature.set(ecs->getComponentType<Engine::PathFinderAgent>());
		signature.set(ecs->getComponentType<Engine::Unit>());
		signature.set(ecs->getComponentType<Engine::Collider>());
		//signature.set(ecs->getComponentType<Engine::Leader>());

		ecs->setSystemSignature<Engine::LeaderSystem>(signature);
	}
	leader_system->init(ecs.get(), unit_system.get(), render_system.get());
#pragma endregion

#pragma region Units
	////! Pathfinding !////
	path_finder = std::make_unique<Engine::PathFinder>(tile_map.get());

	////! Unit System !////
	unit_system =
		ecs->registerSystem<Engine::UnitSystem>();
	{
		Engine::Signature signature;
		signature.set(ecs->getComponentType<Engine::Transform>());
		signature.set(ecs->getComponentType<Engine::RigidBody>());
		signature.set(ecs->getComponentType<Engine::PathFinderAgent>());
		signature.set(ecs->getComponentType<Engine::Unit>());
		signature.set(ecs->getComponentType<Engine::Collider>());
		ecs->setSystemSignature<Engine::UnitSystem>(signature);
	}
	unit_system->init(ecs.get(), tile_map.get(), path_finder.get(), collision_system, render_system);
#pragma endregion

#pragma region Settlement
	////! Settlement System !////
	settle_system =
		ecs->registerSystem<Engine::SettleSystem>();
	{
		Engine::Signature signature;
		signature.set(ecs->getComponentType<Engine::Transform>());
		signature.set(ecs->getComponentType<Engine::Unit>());
		signature.set(ecs->getComponentType<Engine::Settle>());
		ecs->setSystemSignature<Engine::SettleSystem>(signature);
	}
	settle_system->init(ecs.get(), tile_map.get(), path_finder.get(), render_system.get());

	////! Settlement Spawner System !////
	settlement_spawner_system = ecs->registerSystem<Engine::SettlementSpawnerSystem>();
	{
		Engine::Signature signature;
		signature.set(ecs->getComponentType<Engine::Settlement>());

		ecs->setSystemSignature<Engine::SettlementSpawnerSystem>(signature);
	}
	settlement_spawner_system->init(ecs.get(), unit_system.get(),
		[this](const Engine::Entity& collision, const Engine::Entity& collisio) {onCollisionEnterTest(collision, collisio); },
		[this](const Engine::Entity& collision, const Engine::Entity& collisio) {onCollisionExitTest(collision, collisio);  });

#pragma endregion

#pragma region Abilities
	////! Particle System !////
	/// Handles individual particles
	particle_system = ecs->registerSystem<Engine::ParticleSystem>();
	{
		Engine::Signature signature;
		signature.set(ecs->getComponentType<Engine::Particle>());
		signature.set(ecs->getComponentType<Engine::Transform>());
		signature.set(ecs->getComponentType<Engine::RigidBody>());
		signature.set(ecs->getComponentType<Engine::SpriteRenderer>());
		signature.set(ecs->getComponentType<Engine::Lifetime>());

		ecs->setSystemSignature<Engine::ParticleSystem>(signature);
	}
	particle_system->init(ecs.get());

	////! Emitter System !////
	/// Manages particle emitters)
	emitter_system = ecs->registerSystem<Engine::EmitterSystem>();
	{
		Engine::Signature signature;
		signature.set(ecs->getComponentType<Engine::Emitter>());
		signature.set(ecs->getComponentType<Engine::Transform>());
		signature.set(ecs->getComponentType<Engine::RigidBody>());
		signature.set(ecs->getComponentType<Engine::Lifetime>());

		ecs->setSystemSignature<Engine::EmitterSystem>(signature);
	}
	emitter_system->init(ecs.get(), render_system.get());

	////! Energy Decay System !////
	decay_system = ecs->registerSystem<Engine::DecaySystem>();
	{
		Engine::Signature signature;
		signature.set(ecs->getComponentType<Engine::Unit>());
		signature.set(ecs->getComponentType<Engine::EnergyDecay>());

		ecs->setSystemSignature<Engine::DecaySystem>(signature);
	}
	decay_system->init(ecs.get());

	////! Energy Regen System !////
	energy_regen_system = ecs->registerSystem<Engine::EnergyRegenSystem>();
	{
		Engine::Signature signature;
		signature.set(ecs->getComponentType<Engine::Energy>());
		signature.set(ecs->getComponentType<Engine::Settlement>());

		ecs->setSystemSignature<Engine::EnergyRegenSystem>(signature);
	}
	energy_regen_system->init(ecs.get());
#pragma endregion

#pragma region Pathfinding (Continued)
	Engine::PathFinder path_finder(tile_map.get());

	Engine::Vector2Int test_start = tile_map->worldToTile({ 100, 100, 0 });
	Engine::Vector2Int test_end = tile_map->worldToTile({ 300, 300, 0 });

	std::deque<Engine::PathFinder::Node*> path = path_finder.findPath(test_start.x, test_start.y, test_end.x, test_end.y);

	std::cout << path.size() << std::endl;

	while (!path.empty())
	{
		Engine::PathFinder::Node* node = path.back();
		std::cout << node->x << "," << node->y << std::endl;
		path.pop_back();
	}
#pragma endregion

	// Worship system
	worship_system = ecs->registerSystem<Engine::WorshipSystem>();
	{
		Engine::Signature signature;
		signature.set(ecs->getComponentType<Engine::Settlement>());

		ecs->setSystemSignature<Engine::WorshipSystem>(signature);
	}
	worship_system->init(ecs.get(), 0.025F);

	// Blue Team controller system
	team_blue_controller = ecs->createEntity();
	auto& blue_control_comp = ecs->addComponent<Engine::TeamController>(team_blue_controller, {});
	blue_control_comp.assigned_team = Engine::Teams::BLUE;
	auto& blue_mana_pool = ecs->addComponent<Engine::ManaPool>(team_blue_controller, {});
	*blue_mana_pool.mana_pool = 10.0F;
	worship_system->registerController(team_blue_controller);

	// Red Team controller system
	Engine::Entity team_red_controller = ecs->createEntity();
	auto& red_control_comp = ecs->addComponent<Engine::TeamController>(team_red_controller, {});
	red_control_comp.assigned_team = Engine::Teams::RED;
	auto& red_mana_pool = ecs->addComponent<Engine::ManaPool>(team_red_controller, {});
	*red_mana_pool.mana_pool = 10.0F;
	worship_system->registerController(team_red_controller);

	// Ability AI system - Determines what ability the AI shall use
	ability_ai_system = ecs->registerSystem<Engine::AbilityAISystem>();
	{
		Engine::Signature signature;
		signature.set(ecs->getComponentType<Engine::Transform>());
		//When units use Energy component, shrink list with that

		ecs->setSystemSignature<Engine::AbilityAISystem>(signature);
	}
	Engine::AbilityAIData ability_ai_data;
	ability_ai_data.mana_cost_mod = 1.0F;
	ability_ai_system->init(ecs.get(), tile_map.get(), ability_ai_data, red_mana_pool.mana_pool, Engine::Teams::RED);
	addAbilityFireStorm(ability_ai_system);
	addAbilityPillarOfFire(ability_ai_system);

	// ------------------------------ AI Ability handling system ------------------------------
	// This system uses the ability determined by the Ability AI system (see above system)
	ai_ability_system = ecs->registerSystem<Engine::AIAbilitySystem>();
	{
		Engine::Signature signature;
		signature.set(ecs->getComponentType<Engine::Transform>());
		signature.set(ecs->getComponentType<Engine::AbilitySpawner>());

		ecs->setSystemSignature<Engine::AIAbilitySystem>(signature);
	}
	ai_ability_system->init(ecs.get(), ability_manager.get(), tile_map.get(), emitter_system.get());

	////! UI Manager !////
	ui_canvas = ecs->registerSystem<Engine::UiManager>();
	{
		Engine::Signature signature;
		signature.set(ecs->getComponentType<Engine::Transform>());
		signature.set(ecs->getComponentType<Engine::UI>());
		ecs->setSystemSignature<Engine::UiManager>(signature);
	}
	ui_canvas->init(ecs.get(), render_system.get(), text_system.get(), getCurrentScene(), ability_manager.get());
}

void SceneGame::initAudio()
{
	// Button press
	button_press_sound.sound_name   = Engine::Driver::getLoadHandler()->getResourcePath(Engine::LoadHandler::Resources::DEBUG_AUDIO);

	// Pillar of fire
	pillar_of_fire_sound.sound_name = Engine::Driver::getLoadHandler()->getResourcePath(Engine::LoadHandler::Resources::DEBUG_AUDIO);
	
	// NPC death noise
	npc_death_sound.sound_name      = Engine::Driver::getLoadHandler()->getResourcePath(Engine::LoadHandler::Resources::DEBUG_AUDIO);
}

void SceneGame::update(DX::StepTimer const& timer)
{
#pragma region Visual
	ui_canvas->moveCursor(x_input * booster, y_input * booster, timer);
	ui_canvas->update(timer);
	cloud_system->update(timer);
#pragma endregion

	/// ONLY UPDATE CERTAIN SYSTEMS WHEN NOT PAUSED!
	if (!sceneData.pause_scene)
	{
#pragma region Physics/Rigidbodies
		physics_system->update(timer);
		boundary_system->update(timer);
		collision_system->update(timer);
#pragma endregion

#pragma region AI
	unit_system->update(timer);
	ai_settlement_watch_system->update(timer);
	ai_sprog_system->update(timer);
	flag_creation_system->update(timer);
	flag_raising_system->update(timer);
	ability_ai_system->update(timer);
#pragma endregion

#pragma region Settlement
		settle_system->update(timer);
		settlement_spawner_system->update(timer);
#pragma endregion

#pragma region Abilities
		energy_regen_system->update(timer);
		decay_system->update(timer);
		emitter_system->update(timer);
		particle_system->update(timer);
		ai_ability_system->update(timer);
		leader_system->update(timer);

		auto& blue_mana_pool = ecs->getComponent<Engine::ManaPool>(team_blue_controller);
		ui_canvas->updateManaPool(*blue_mana_pool.mana_pool);
#pragma endregion
	}

	/// Update the ECS
	ecs->update();
}

void SceneGame::render(DX::StepTimer const& timer)
{
	scale_system->update(timer);
	render_system->update(timer);
	text_system->update();
}

void SceneGame::onKey(const DirectX::Keyboard::KeyboardStateTracker& keyboard_data)
{
	// Return to menu upon hitting escape
	if (keyboard_data.pressed.Escape || keyboard_data.pressed.C)
	{
		setNewScene(GameScenes::MAIN_MENU);
	}

	// Pause the game
	if (keyboard_data.pressed.Space)
	{
		setPause(!sceneData.pause_scene);
	}

	if (keyboard_data.released.A)
	{
		auto window = Engine::Driver::getDriver()->getWindowSize();
		Engine::Entity unit = ecs->createEntity();
		unit_system->initAgent(
			unit,                                                                                                                  // The entity 
			{ DirectX::SimpleMath::Vector3(window.x * 0.6F, window.y * 0.35F, 0), DirectX::SimpleMath::Vector2::One, 0 },                                  // Transform 
			{ DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0, 0, 0), 0 },                                   // Rigidbody
			{ Engine::Teams::RED, Engine::Unit::UnitType::Normal },                                                      // Unit
			Engine::Driver::getLoadHandler()->getResourcePath(Engine::LoadHandler::Resources::CHARACTER_TEXTURE) ,     // Sprite texture path
			[this](const Engine::Entity& collision, const Engine::Entity& collisio) {onCollisionEnterTest(collision, collisio); }, // On collision enter 
			[this](const Engine::Entity& collision, const Engine::Entity& collisio) {onCollisionExitTest(collision, collisio);  }, // On collision exit
			{ 10, 10 },                                                                                                            // Collider size
			200,                                                                                                                   // AI speed
			20);                                                                                                                   // AI sensitivity 
	}

	if (keyboard_data.released.S)
	{
		auto window = Engine::Driver::getDriver()->getWindowSize();
		Engine::Entity unit = ecs->createEntity();
		unit_system->initAgent(
			unit,                                                                                                                  // The entity 
			{ DirectX::SimpleMath::Vector3(window.x * 0.6F, window.y * 0.75F, 0), DirectX::SimpleMath::Vector2::One, 0 },                                  // Transform 
			{ DirectX::SimpleMath::Vector3(0, 0, 0), DirectX::SimpleMath::Vector3(0, 0, 0), 0 },                                   // Rigidbody
			{ Engine::Teams::BLUE, Engine::Unit::UnitType::Normal },                                                      // Unit
			Engine::Driver::getLoadHandler()->getResourcePath(Engine::LoadHandler::Resources::ENEMY_TEXTURE),     // Sprite texture path
			[this](const Engine::Entity& collision, const Engine::Entity& collisio) {onCollisionEnterTest(collision, collisio); }, // On collision enter 
			[this](const Engine::Entity& collision, const Engine::Entity& collisio) {onCollisionExitTest(collision, collisio);  }, // On collision exit
			{ 10, 10 },                                                                                                            // Collider size
			200,                                                                                                                   // AI speed
			20);                                                                                                                   // AI sensitivity 
	}

	// Arcade machine input
	if (keyboard_data.pressed.Z)
	{
		inputLeftClick();
	}
	if (keyboard_data.pressed.X)
	{
		inputRightClick();
	}

	if (keyboard_data.pressed.LeftShift)
	{
		booster = 2.5F;
	}
	else if (keyboard_data.released.LeftShift)
	{
		booster = 1.0F;
	}

	if (keyboard_data.pressed.Left && x_input == 0.0F)
	{
		x_input = -1.0F;
	}
	else if (keyboard_data.released.Left && x_input <= 0.0F)
	{
		x_input = 0.0F;
	}

	if (keyboard_data.pressed.Right && x_input == 0.0F)
	{
		x_input = 1.0F;
	}
	else if (keyboard_data.released.Right && x_input >= 0.0F)
	{
		x_input = 0.0F;
	}

	if (keyboard_data.pressed.Up && y_input == 0.0F)
	{
		y_input = -1.0F;
	}
	else if (keyboard_data.released.Up && y_input <= 0.0F)
	{
		y_input = 0.0F;
	}

	if (keyboard_data.pressed.Down && y_input == 0.0F)
	{
		y_input = 1.0F;
	}
	else if (keyboard_data.released.Down && y_input >= 0.0F)
	{
		y_input = 0.0F;
	}
}

void SceneGame::onMouse(const DirectX::Mouse::ButtonStateTracker& mouse_data)
{
	if (mouse_data.leftButton == mouse_data.PRESSED)
	{
		inputLeftClick();
	}
	if (mouse_data.rightButton == mouse_data.PRESSED)
	{
		inputRightClick();
	}
}

void SceneGame::setPause(const bool& pause)
{
	Scene::setPause(pause);
	render_system->setPlaySpriteAnimations(!pause);
}

void SceneGame::inputLeftClick()
{
	Vector2 cur_pos2d = Vector2(ui_canvas->getCursorCoordinates());

	if (!ui_canvas->checkCollision(Vector3{ cur_pos2d.x, cur_pos2d.y, 0.0f }))
	{
		if (ability_manager->IsAffectingMap())
		{
			// tile_map->changeTiles(ui_canvas->getCursorCoordinates(), render_system.get(), tile_textures, 1);

			Engine::Vector2Int click_pos       = tile_map->worldToTile(ui_canvas->getCursorCoordinates());
			Engine::TileMap::Tile* parent_tile = tile_map->findTile(click_pos.x, click_pos.y);
			tile_map->changeTilesRecursive(*parent_tile, render_system, tile_textures, 1);
		}
		else if (ability_manager->IsSummoningHero())
		{
			std::cout << "HERO SUMMONING ABILITY";
		}
		else if (ability_manager->UseAbility(cur_pos2d))
		{
			Engine::Emitter emitter = ability_manager->getEmitterData();
			emitter.location = cur_pos2d;

			Vector3 velocity(0, 0, 0);
			emitter_system->createEmitter(emitter, velocity);
		}
	}
}

void SceneGame::inputRightClick()
{
	Vector2 cur_pos2d = Vector2(ui_canvas->getCursorCoordinates());

	if (!ui_canvas->checkCollision(Vector3{ cur_pos2d.x, cur_pos2d.y, 0.0f }))
	{
		if (ability_manager->IsAffectingMap())
		{
			tile_map->changeTiles(ui_canvas->getCursorCoordinates(), render_system.get(), tile_textures, -1);
		}
	}
}

void SceneGame::createUnits(const unsigned int number_of_units)
{
	for (unsigned int i = 0; i < number_of_units; ++i)
	{
		Engine::Entity unit = ecs->createEntity();
		Engine::Transform location;
		DirectX::SimpleMath::Vector2 window_size = Engine::Driver::getDriver()->getWindowSize();
		location.position = DirectX::SimpleMath::Vector3(window_size.x / 2,
			(window_size.y / 2) - (GRID_SIZE.y * TILE_SIZE.y / 2), 10);
		Engine::RigidBody rigid;
		rigid.acceleration = DirectX::SimpleMath::Vector3(0, 0, 0);
		rigid.velocity = DirectX::SimpleMath::Vector3(0, 0, 0);
		rigid.gravity_scale = 0;

		unit_system->initAgent(
			unit,																																																// The entity 
			location,																																															// Transform 
			rigid,																																																// Rigidbody
			{ Engine::Teams::RED, Engine::Unit::UnitType::Normal },																									// Unit
			Engine::Driver::getLoadHandler()->getResourcePath(Engine::LoadHandler::Resources::CHARACTER_TEXTURE),// Sprite texture path
			[this](const Engine::Entity& collision, const Engine::Entity& collisio) {onCollisionEnterTest(collision, collisio); }, // On collision enter 
			[this](const Engine::Entity& collision, const Engine::Entity& collisio) {onCollisionExitTest(collision, collisio);  },	// On collision exit
			{ 10, 10 },																																														// Collider size
			20,																																																	// AI speed
			10);																																																	// AI sensitivity 

		if (i == 0)
		{
			//collision_system->deinitCollider(unit);
			ecs->destroyEntity(unit); // Just so we don't get a pink square for the demo
		}
	}
}

void SceneGame::onCollisionEnterTest(const Engine::Entity& t, const Engine::Entity& v)
{
	std::cout << "Collision Enter!" << rand() % 100 << std::endl;

	if (ecs->hasComponent<Engine::Unit>(t) && ecs->hasComponent<Engine::Particle>(v))
	{
		Engine::Particle& particle = ecs->getComponent<Engine::Particle>(v);
		if (particle.damaging)
		{
			ecs->destroyEntityAtEndOfFrame(t);
		}
	}

	if (ecs->hasComponent<Engine::Unit>(t) && ecs->hasComponent<Engine::Emitter>(v))
	{
		Engine::Emitter& emitter = ecs->getComponent<Engine::Emitter>(v);
		if (emitter.damage > 0.0F)
		{
			ecs->destroyEntityAtEndOfFrame(t);
		}
	}

	if (ecs->hasComponent<Engine::Unit>(t) && ecs->hasComponent<Engine::Unit>(v))
	{
		Engine::Unit& unit1 = ecs->getComponent<Engine::Unit>(t);
		Engine::Unit& unit2 = ecs->getComponent<Engine::Unit>(v);

		if (unit1.unit_team == unit2.unit_team)
		{
			unit_system->merge(t, v);
		}
		else
		{
			unit_system->fight(t, v);
			Engine::Transform& unit_1 = ecs->getComponent<Engine::Transform>(t);
			emitter_system->createFigthCloud( {unit_1.position.x, unit_1.position.y} );
		}
	}
}

void SceneGame::onCollisionExitTest(const Engine::Entity& t, const Engine::Entity& v)
{

}

void SceneGame::addAbilityFireStorm(std::shared_ptr<Engine::AbilityAISystem> target_ai)
{
	Engine::AIAbilityInfo fire_storm;
	fire_storm.ability = Engine::AbilityTypes::FIRE_STORM;
	fire_storm.cast_delay = 1.0F;
	fire_storm.cast_delay_variability = 2.0F;
	fire_storm.mana_cost = 5.0F;
	//=== Ability Selection ===
	//More likely to cast at high mana
	fire_storm.mana_modifier = 0.1F;
	fire_storm.mana_multiplier = 1.0F;
	//More likely per enemy settlement
	fire_storm.ab_oth_settlement_mod = 5.0F;
	fire_storm.ab_oth_settlement_mult = 1.1F;
	//Less likely per own settlement
	fire_storm.ab_own_settlement_mod = -4.0F;
	fire_storm.ab_own_settlement_mult = 0.91F;
	//More likely per enemy unit
	fire_storm.ab_oth_unit_mod = 1.0F;
	fire_storm.ab_oth_unit_mult = 1.01F;
	//Own unit neutral
	fire_storm.ab_own_unit_mod = 0.0F;
	fire_storm.ab_own_unit_mult = 1.0F;

	//=== Target Selection ===
	//Divide map into 2x2 boxes
	fire_storm.segmentation = 2;
	//Target segments with enemy settlements
	fire_storm.map_oth_settlement_mod = 5.0F;
	fire_storm.map_oth_settlement_mult = 2.0F;
	//Don't target segments with own settlements
	fire_storm.map_own_settlement_mod = 0.0F;
	fire_storm.map_own_settlement_mult = 0.0F;
	//More likely to target segments with enemy units
	fire_storm.map_oth_unit_mod = 2.5F;
	fire_storm.map_oth_unit_mult = 1.1F;
	//Less likely to target segments with own units
	fire_storm.map_own_unit_mod = -2.5F;
	fire_storm.map_own_unit_mult = 1.0F;

	target_ai->addAbility(fire_storm);
}

void SceneGame::addAbilityPillarOfFire(std::shared_ptr<Engine::AbilityAISystem> target_ai)
{
	Engine::AIAbilityInfo pillar_of_fire;
	pillar_of_fire.ability = Engine::AbilityTypes::PILLAR_OF_FIRE;
	pillar_of_fire.cast_delay = 1.0F;
	pillar_of_fire.cast_delay_variability = 2.0F;
	pillar_of_fire.mana_cost = 5.0F;
	//=== Ability Selection ===
	//More likely to cast at high mana
	pillar_of_fire.mana_modifier = 0.1F;
	pillar_of_fire.mana_multiplier = 1.0F;
	//More likely per enemy settlement
	pillar_of_fire.ab_oth_settlement_mod = 1.0F;
	pillar_of_fire.ab_oth_settlement_mult = 1.01F;
	//Less likely per own settlement
	pillar_of_fire.ab_own_settlement_mod = -4.0F;
	pillar_of_fire.ab_own_settlement_mult = 0.91F;
	//More likely per enemy unit
	pillar_of_fire.ab_oth_unit_mod = 5.0F;
	pillar_of_fire.ab_oth_unit_mult = 1.1F;
	//Own unit neutral
	pillar_of_fire.ab_own_unit_mod = 0.0F;
	pillar_of_fire.ab_own_unit_mult = 1.0F;

	//=== Target Selection ===
	//Divide map into 2x2 boxes
	pillar_of_fire.segmentation = 1;
	//Target segments with enemy settlements
	pillar_of_fire.map_oth_settlement_mod = 2.5F;
	pillar_of_fire.map_oth_settlement_mult = 1.1F;
	//Don't target segments with own settlements
	pillar_of_fire.map_own_settlement_mod = 0.0F;
	pillar_of_fire.map_own_settlement_mult = 0.0F;
	//More likely to target segments with enemy units
	pillar_of_fire.map_oth_unit_mod = 5.0F;
	pillar_of_fire.map_oth_unit_mult = 2.0F;
	//Less likely to target segments with own units
	pillar_of_fire.map_own_unit_mod = -2.5F;
	pillar_of_fire.map_own_unit_mult = 1.0F;

	target_ai->addAbility(pillar_of_fire);
}
