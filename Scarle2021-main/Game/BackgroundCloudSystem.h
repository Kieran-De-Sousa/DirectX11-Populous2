#pragma once

// DirectX
#include <SimpleMath.h>
#include "StepTimer.h"

// Project
#include "System.h"
#include "Coordinator.h"

#include "RenderSystem.h"

namespace Engine
{
	/// <summary>
	/// Moves clouds across the screen
	/// Because we have an isometric view, the tilemap 
	/// </summary>
	class BackgroundCloudSystem : public Engine::System
	{
	public:
		BackgroundCloudSystem();
		~BackgroundCloudSystem() = default;

		/// <summary>
		/// Init the system, systems can't have constructors so required after creation
		/// </summary>
		/// <param name="_coordinator"> | The scene's coordinator</param>
		/// <param name="_screen_dimensions"> | The window dimensions </param>
		/// <param name="_cloud_move_speed"> | How fast the clouds will move </param>
		void init(Coordinator* _coordinator, const DirectX::SimpleMath::Vector2& _screen_dimensions, float _cloud_move_speed);
		/// <summary>
		/// Updates the system, moving the clouds across the screen 
		/// </summary>
		/// <param name="timer">Driver->Scene's StepTimer</param>
		void update(const DX::StepTimer& timer);

		/// <summary>
		/// Data structure which holds the infomatin for setting up a mutliple lcoud background
		/// </summary>
		struct CloudsCreationData
		{
			std::string cloud_path = "Texture in BackgroundCloudSystem->CloudCreationData wasn't set!"; // The file path of the cloud texture
			 
			float cloud_scale = 0;                                                                      // How big the cloud sound be
			float x_increment = 0;                                                                      // The x offset between clouds
			float y_increment = 0;                                                                      // The y offset between clouds

			int z_order = -100; // The clouds should be behind everything else                          // The z order of the cloud
		};
		/// <summary>
		/// Creates a single cloud background
		/// </summary>
		/// <param name="render_system"> | Pointer to the scene's render system </param>
		/// <param name="settings"> | CloudCreationData settings </param>
		void createClouds(const std::shared_ptr<RenderSystem>& render_system, const CloudsCreationData& settings);

		/// <summary>
		/// Data structure which holds the infomation for setting up a single cloud background
		/// </summary>
		struct CloudCreationData
		{
			std::vector<std::string> cloud_images{};                            // A vector of all of the cloud textures
			 
			float cloud_spacing = 0;                                            // How far apart should clouds be
			 
			int z_order = -100; // The clouds should be behind everything else  // The z order of the clouds
		};
		/// <summary>
		/// Creates a multiple cloud background
		/// </summary>
		/// <param name="render_system"> | Pointer to the scene's render system </param>
		/// <param name="settings"> CloudsCreationData settings </param>
		void createCloud(const std::shared_ptr<RenderSystem>& render_system, const CloudCreationData& settings);

	private:
		/// <summary>
		/// Pointer to the scene's coordinator
		/// </summary>
		Coordinator* coordinator;

		/// <summary>
		/// The window dimensions 
		/// </summary>
		DirectX::SimpleMath::Vector2 screen_dimensions{};

		/// <summary>
		/// How fast the clouds should move 
		/// </summary>
		float cloud_move_speed = 0;
	};
} // namespace Engine
