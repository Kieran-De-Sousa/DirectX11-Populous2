// Created by Ben on 28/02/2022

#pragma once

#include "DrawData2D.h"
#include "CommonStates.h"
#include "StepTimer.h"

#include "System.h"
#include "Coordinator.h"

// Render stuff

namespace Engine
{
	/// <summary>
	/// Handles the rendering, initialisation, and all other operations regarding Sprites
	/// </summary>
	class RenderSystem : public Engine::System
	{
	public:
		RenderSystem();
		~RenderSystem() = default;

		/// <summary>
		/// Init the system, systems can't have constructors so required after creation
		/// </summary>
		/// <param name="_coordinator"> | The scene's coordinator </param>
		/// <param name="_main_camera"> | The scene's main camera </param>
		void init(Coordinator* _coordinator, const Engine::Entity& _main_camera);
		/// <summary>
		/// Update the system, rendering all sprites and ticking their animations
		/// </summary>
		/// <param name="timer"> | Drivers->Scene's steptimer </param>
		void update(DX::StepTimer const& timer);

		/// <summary>
		/// Initialise a sprite with the given texture, first checking if the texture is already loaded
		/// with loadTexture()
		/// </summary>
		/// <param name="texture_path"> | The resource path of the texture </param>
		/// <param name="entity"> | The entities whos sprite to init </param>
		void initSprite(const std::string& texture_path, const Entity& entity);
		/// <summary>
		/// Update a sprites texture from the given path, first checking the if texture is already loaded 
		/// with loadTexture()
		/// </summary>
		/// <param name="texture_path"> | The resource path of the texture </param>
		/// <param name="entity"> | The entities who sprite is changed </param>
		void updateSprite(const std::string& texture_path, const Entity& entity);
		/// <summary>
		/// Remove a sprite from an entity
		/// </summary>
		/// <param name="entity"> | The enitity whos sprite is to be removed </param>
		void deinitSprite(const Entity& entity);

		/// <summary>
		/// Initialise a sprites animator
		/// </summary>
		/// <param name="entity"> | The entity whos sprite component to initialise with an animator </param>
		/// <param name="_animations"> | A map of animation name - vector of bounding boxes for the animations to allow for easy animation switching </param>
		/// <param name="_tick_speed"> | The speed the animation will play</param>
		void initAnimator(const Entity& entity, const std::unordered_map<std::string, std::vector<std::array<LONG, 4>>>& _animations, double _tick_speed);
		/// <summary>
		/// Change the sprites animation
		/// </summary>
		/// <param name="entity"> | The entity who's sprite animation to change </param>
		/// <param name="animation_name"> | The name of the animation to change to</param>
		void changeAnimation(const Entity& entity, const std::string& animation_name);

		/// <summary>
		/// Reorder the spites rendering based off their z order 
		/// </summary>
		void refreshSpriteZOrder();

		/// <summary>
		/// Return play_sprite_animations
		/// (Used to control if sprite's animators will be ticked)
		/// </summary>
		/// <returns></returns>
		bool getPlaySpriteAnimations() const;
		/// <summary>
		/// Set play_sprite_animations 
		/// (Used to control if sprite's animators will be ticked)
		/// </summary>
		/// <param name="_play_sprite_animations"></param>
		void setPlaySpriteAnimations(bool _play_sprite_animations);

	private:
		/// <summary>
		/// Tick a sprites animator, changing the frame (bounding box) if needed
		/// </summary>
		/// <param name="entity"> | The entity whos sprite's animator to tick </param>
		/// <param name="delta_time"> | How much to tick the animator by </param>
		void updateSpriteAnimator(const Entity& entity, double delta_time);

		/// <summary>
		/// A struct which is used in the loadTexture operation 
		/// </summary>
		struct LoadedTexture
		{
			ID3D11ShaderResourceView* texture; // The texture
			bool successful;                   // Was the load of the texture successful?
		};
		/// <summary>
		/// Loads a texture from a file path, checking if its already been loading first
		/// </summary>
		/// <param name="texture_path"> | The file path of the texture </param>
		/// <returns>A LoadTexture struct (texture - successful load) </returns>
		LoadedTexture loadTexture(const std::string& texture_path);

		/// <summary>
		/// Pointer to the scene's coordinator
		/// </summary>
		Coordinator* coordinator;
		/// <summary>
		/// Pointer to the Driver's draw data
		/// </summary>
		DrawData2D* draw_data;
		/// <summary>
		/// Pointer to the Driver's common states 
		/// </summary>
		CommonStates* common_states;
		/// <summary>
		/// Pointer to the Driver's d3d devive
		/// </summary>
		ID3D11Device1* d3d_device;
		
		/// <summary>
		/// Index of the scene's main camera
		/// </summary>
		Engine::Entity main_camera = NULL_ENTITY;
		/// <summary>
		/// Map of loaded texture paths to texture
		/// </summary>
		std::unordered_map<std::string, ID3D11ShaderResourceView*> textures;

		/// <summary>
		/// Controls if sprite's animators should be ticked 
		/// </summary>
		bool play_sprite_animations = true;
	};
}
