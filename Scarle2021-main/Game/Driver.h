// Created by Ben on 28/02/2022
// Based upon the existing 'Game' class in the ScarleEngine

#pragma once

// DirextX
#include <SimpleMath.h>
#include "StepTimer.h"
#include "CommonStates.h"
#include "../DirectXTK/Inc/Effects.h" // This clashes with a SDK file so we must explitily state it

// System
#include <list>

// Driver
#include "SceneHandler.h"
#include "InputHandler.h"
#include "AudioHandler.h"
#include "LoadHandler.h"

// ECS
#include "Cam.h"          // Needed to create the draw data
// Needed to allow the RenderSystems to access direct x magic to render sprites
#include "RenderSystem.h" 
#include "TextSystem.h"
#include "AutoScaleSystem.h"

using std::list;

// Forward declarations
struct GameData;
struct DrawData;
struct DrawData2D;
class GameObject;
class GameObject2D;
class Camera;
class TPSCamera;
class Light;
class Sound;

namespace Engine
{
    class Driver
    {
    public:
        Driver() noexcept;
        ~Driver() = default;

        Driver(Driver&&) = default;
        Driver& operator= (Driver&&) = default;

        Driver(Driver const&) = delete;
        Driver& operator= (Driver const&) = delete;

        // Initialization and management
        void initialize(HWND _window, int _width, int _height);

        // Basic game loop
        void tick();

        // Messages
        void onActivated();
        void onDeactivated();
        void onSuspending();
        void onResuming();
        void onWindowSizeChanged(int _width, int _height);
        void onExit();

        // Properties
        void setDefaultSize(int& _width, int& _height);
        void setWindowSize(const int& _width, const int& _height) { onWindowSizeChanged(_width, _height); }

        DirectX::SimpleMath::Vector2 getDefaultSize() const { return default_window_size; }
        DirectX::SimpleMath::Vector2 getWindowSize() const;

        // Properties++
        static Driver*       getDriver();
        static SceneHandler* getSceneHandler();
        static LoadHandler*  getLoadHandler();
        static AudioHandler* getAudioHandler();

        // I don't know a better way of linking up 
        void createDrawData(Engine::Camera* camera, Light* light);

        /// "Hacking" the system to expose the game data object
        GameData* getGameData();
        DrawData* getDrawData();

    private:
        // Main loops
        void update(DX::StepTimer const& timer);
        void render();

        // DirectX Magic
        void clear();
        void present();

        void createDevice();
        void createResources();
        void onDeviceLost();

        // Device resources
        HWND                                            hwnd_window;
        int                                             output_width;
        int                                             output_height;

        D3D_FEATURE_LEVEL                               feature_level;
        Microsoft::WRL::ComPtr<ID3D11Device1>           d3d_device;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    d3d_context;
        
        Microsoft::WRL::ComPtr<IDXGISwapChain1>         swap_chain;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  render_target_view;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  depth_stencil_view;

        // Rendering loop timer
        DX::StepTimer                                   step_timer;

        // Scarle Wizardry 
        GameData* game_data = nullptr;			 // Data to be shared to all Game Objects as they are ticked
        DrawData* draw_data = nullptr;           // Even though we are doing 3D we need this
        DrawData2D* draw_data_2D = nullptr;	     // Data to be passed by game to all 2D Game Objects via Draw 

        // Required for the CMO model rendering system
        DirectX::CommonStates* states = nullptr;
        DirectX::IEffectFactory* fx_factory = nullptr;

        // Default settings
        const DirectX::SimpleMath::Vector2 WINDOW_SIZE = DirectX::SimpleMath::Vector2(800, 600); // Minimum size is 320x200
        DirectX::SimpleMath::Vector2 current_window_size = WINDOW_SIZE;
        DirectX::SimpleMath::Vector2 default_window_size = WINDOW_SIZE;

        // Driver
        std::unique_ptr<SceneHandler> scene_handler;
        std::unique_ptr<InputHandler> input_handler;
        std::unique_ptr<AudioHandler> audio_handler;
        std::unique_ptr<LoadHandler>  load_handler;

        static Driver* instance;

        friend Engine::RenderSystem; // Got to be a better way of doing this?? maybe..
        friend Engine::TextSystem;
        friend Engine::AutoScaleSystem;
    };
}
