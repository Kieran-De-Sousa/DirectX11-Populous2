// Created by Ben on 28/02/2022
// GAME GOES TO GOLD!!!!

#include "pch.h"

#include "GameData.h"
#include "GameState.h"
#include "DrawData.h"
#include "DrawData2D.h"
#include "ObjectList.h"

#include <time.h>
#include <iostream>

#include "Driver.h"

extern void ExitGame() noexcept;

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Engine::Driver* Engine::Driver::instance = nullptr; // idk dude i like singletons.. or in this case pseudo singletons...

Engine::Driver::Driver() noexcept :
    hwnd_window(nullptr),
    output_width(static_cast<int>(WINDOW_SIZE.x)),
    output_height(static_cast<int>(WINDOW_SIZE.y)),
    feature_level(D3D_FEATURE_LEVEL_11_0)
{
}

// Initialize the Direct3D resources required to run, setup the ECS and load the first scene
void Engine::Driver::initialize(HWND _window, int _width, int _height)
{
    if (instance != nullptr)
    {
        std::cout << "Why are you trying to init two drivers?" << std::endl;
        return;
    }

    instance = this;

#pragma region Direct X / Scarle Magic
    hwnd_window = _window;
    output_width = std::max(_width, 1);
    output_height = std::max(_height, 1);
    current_window_size = DirectX::SimpleMath::Vector2(output_width, output_height);

    createDevice();
    createResources();

    // Seed the random number generator
    srand((UINT)time(nullptr));

    // Create GameData struct and populate its pointers
    game_data = new GameData;
    game_data->m_GS = GS_PLAY_MAIN_CAM; // simon cmon dude

    // Set up systems for 2D rendering
    draw_data_2D = new DrawData2D();
    draw_data_2D->m_Sprites.reset(new SpriteBatch(d3d_context.Get()));
    draw_data_2D->m_Font.reset(new SpriteFont(d3d_device.Get(), L"..\\Assets\\italic.spritefont"));
    states = new CommonStates(d3d_device.Get());

    // Set up DirectXTK Effects system
    fx_factory = new EffectFactory(d3d_device.Get());
    // Tell the fxFactory to look to the correct build directory to pull stuff in from
    ((EffectFactory*)fx_factory)->SetDirectory(L"..\\Assets");
    // Init render system for VBGOs
    VBGO::Init(d3d_device.Get());

#pragma region ECS Wizardry 
    // Now in the scenes
#pragma endregion

#pragma region Driver.. stuff
    // Init the handlers
    load_handler = std::make_unique<Engine::LoadHandler>();
    scene_handler = std::make_unique<Engine::SceneHandler>();
    input_handler = std::make_unique<Engine::InputHandler>(&hwnd_window, game_data, scene_handler.get());
    audio_handler = std::make_unique<Engine::AudioHandler>(game_data);
#pragma endregion
}

// Executes the basic game loop
void Engine::Driver::tick()
{
    step_timer.Tick([&]()
    {
        update(step_timer);
    });

    render();
}

// Updates the world
void Engine::Driver::update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetElapsedSeconds());
    game_data->m_dt = elapsedTime;

    // Update the handlers
    input_handler->update(timer);
    scene_handler->update(timer);
    audio_handler->update(timer);
}

// Draws the scene.
void Engine::Driver::render()
{
    // Don't try to render anything before the first Update
    if (step_timer.GetFrameCount() == 0)
    {
        return;
    }

    if (draw_data == nullptr)
    {
        std::cout << "Draw data is null!" << std::endl;
        return;
    }

    if (draw_data->m_cam == nullptr)
    {
        std::cout << "Draw data cam is null!" << std::endl;
        return;
    }

    clear();
    /// PASS IN BOTH DRAW DATAS

    // Set immediate context of the graphics device
    draw_data->m_pd3dImmediateContext = d3d_context.Get();

    //update the constant buffer for the rendering of VBGOs
    VBGO::UpdateConstantBuffer(draw_data);

    //draw_data_2D->m_Sprites->Begin(SpriteSortMode_Deferred, states->NonPremultiplied());
    // Update the handlers
    scene_handler->render(step_timer);

    // Drawing text screws up the Depth Stencil State, this puts it back again!
    d3d_context->OMSetDepthStencilState(states->DepthDefault(), 0);

    present();
}

// Helper method to clear the back buffers
void Engine::Driver::clear()
{
    // Clear the views
    d3d_context->ClearRenderTargetView(render_target_view.Get(), Colors::CornflowerBlue);
    d3d_context->ClearDepthStencilView(depth_stencil_view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    d3d_context->OMSetRenderTargets(1, render_target_view.GetAddressOf(), depth_stencil_view.Get());

    // Set the viewport
    CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(output_width), static_cast<float>(output_height));
    d3d_context->RSSetViewports(1, &viewport);
}

// Presents the back buffer contents to the screen
void Engine::Driver::present()
{
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen
    HRESULT hr = swap_chain->Present(1, 0);

    // If the device was reset we must completely reinitialize the renderer
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        onDeviceLost();
    }
    else
    {
        DX::ThrowIfFailed(hr);
    }
}

// Message handlers
void Engine::Driver::onActivated()
{
    // TODO: Game is becoming active window
}

void Engine::Driver::onDeactivated()
{
    // TODO: Game is becoming background window
}

void Engine::Driver::onSuspending()
{
    // TODO: Game is being power-suspended (or minimized)
}

void Engine::Driver::onResuming()
{
    step_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize)
}

void Engine::Driver::onWindowSizeChanged(int _width, int _height)
{
    output_width = std::max(_width, 1);
    output_height = std::max(_height, 1);
    current_window_size = DirectX::SimpleMath::Vector2(output_width, output_height);

    createResources();

    // TODO: Game window is being resized
}

void Engine::Driver::onExit()
{
    ExitGame();
}

// Properties
void Engine::Driver::setDefaultSize(int& _width, int& _height)
{
    _width = static_cast<int>(WINDOW_SIZE.x);
    _height = static_cast<int>(WINDOW_SIZE.y);
    default_window_size = DirectX::SimpleMath::Vector2(_width, _height);
}
DirectX::SimpleMath::Vector2 Engine::Driver::getWindowSize() const
{
    return current_window_size;
}

// Properties++
Engine::Driver* Engine::Driver::getDriver()
{
    return instance;
}

Engine::SceneHandler* Engine::Driver::getSceneHandler()
{
    return instance->scene_handler.get();
}

Engine::LoadHandler* Engine::Driver::getLoadHandler()
{
    return instance->load_handler.get();
}

Engine::AudioHandler* Engine::Driver::getAudioHandler()
{
    return instance->audio_handler.get();
}

void Engine::Driver::createDrawData(Engine::Camera* camera, Light* light)
{
    // Create DrawData struct and populate its pointers
    
    draw_data = new DrawData;
    draw_data->m_pd3dImmediateContext = nullptr;
    draw_data->m_states = states;
    draw_data->m_cam = camera;
    draw_data->m_light = light;
}

// These are the resources that depend on the device
void Engine::Driver::createDevice()
{
    UINT creationFlags = 0;

#ifdef _DEBUG
    //creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
    //something missing on the machines in 2Q28
    //this should work!
#endif

    static const D3D_FEATURE_LEVEL featureLevels[] =
    {
        // TODO: Modify for supported Direct3D feature levels
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    // Create the DX11 API device object, and get a corresponding context.
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
    DX::ThrowIfFailed(D3D11CreateDevice(
        nullptr,                            // Specify nullptr to use the default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        featureLevels,
        static_cast<UINT>(std::size(featureLevels)),
        D3D11_SDK_VERSION,
        device.ReleaseAndGetAddressOf(),    // Returns the Direct3D device created
        &feature_level,                    // Returns feature level of device created
        context.ReleaseAndGetAddressOf()    // Returns the device immediate context
    ));

#ifndef NDEBUG
    ComPtr<ID3D11Debug> d3dDebug;
    if (SUCCEEDED(device.As(&d3dDebug)))
    {
        ComPtr<ID3D11InfoQueue> d3dInfoQueue;
        if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
        {
#ifdef _DEBUG
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
            D3D11_MESSAGE_ID hide[] =
            {
                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
                // TODO: Add more message IDs here as needed
            };
            D3D11_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumIDs = static_cast<UINT>(std::size(hide));
            filter.DenyList.pIDList = hide;
            d3dInfoQueue->AddStorageFilterEntries(&filter);
        }
    }
#endif

    DX::ThrowIfFailed(device.As(&d3d_device));
    DX::ThrowIfFailed(context.As(&d3d_context));

    // TODO: Initialize device dependent objects here (independent of window size)
}

// Allocate all memory resources that change on a window SizeChanged event
void Engine::Driver::createResources()
{
    // Clear the previous window size specific context
    ID3D11RenderTargetView* nullViews[] = { nullptr };
    d3d_context->OMSetRenderTargets(static_cast<UINT>(std::size(nullViews)), nullViews, nullptr);
    render_target_view.Reset();
    depth_stencil_view.Reset();
    d3d_context->Flush();

    const UINT backBufferWidth = static_cast<UINT>(output_width);
    const UINT backBufferHeight = static_cast<UINT>(output_height);
    const DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
    const DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    constexpr UINT backBufferCount = 2;


    // If the swap chain already exists, resize it, otherwise create one
    if (swap_chain)
    {
        HRESULT hr = swap_chain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
            // If the device was removed for any reason, a new device and swap chain will need to be created
            onDeviceLost();

            // Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
            // and correctly set up the new device
            return;
        }
        else
        {
            DX::ThrowIfFailed(hr);
        }
    }
    else
    {
        // First, retrieve the underlying DXGI Device from the D3D Device
        ComPtr<IDXGIDevice1> dxgiDevice;
        DX::ThrowIfFailed(d3d_device.As(&dxgiDevice));

        // Identify the physical adapter (GPU or card) this device is running on
        ComPtr<IDXGIAdapter> dxgiAdapter;
        DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

        // And obtain the factory object that created it
        ComPtr<IDXGIFactory2> dxgiFactory;
        DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

        // Create a descriptor for the swap chain
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.Width = backBufferWidth;
        swapChainDesc.Height = backBufferHeight;
        swapChainDesc.Format = backBufferFormat;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = backBufferCount;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
        fsSwapChainDesc.Windowed = TRUE;

        // Create a SwapChain from a Win32 window
        DX::ThrowIfFailed(dxgiFactory->CreateSwapChainForHwnd(
            d3d_device.Get(),
            hwnd_window,
            &swapChainDesc,
            &fsSwapChainDesc,
            nullptr,
            swap_chain.ReleaseAndGetAddressOf()
        ));

        // This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut
        DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(hwnd_window, DXGI_MWA_NO_ALT_ENTER));
    }

    // Obtain the backbuffer for this window which will be the final 3D rendertarget
    ComPtr<ID3D11Texture2D> backBuffer;
    DX::ThrowIfFailed(swap_chain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

    // Create a view interface on the rendertarget to use on bind
    DX::ThrowIfFailed(d3d_device->CreateRenderTargetView(backBuffer.Get(), nullptr, render_target_view.ReleaseAndGetAddressOf()));

    // Allocate a 2-D surface as the depth/stencil buffer and create a DepthStencil view on this surface to use on bind
    CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

    ComPtr<ID3D11Texture2D> depthStencil;
    DX::ThrowIfFailed(d3d_device->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    DX::ThrowIfFailed(d3d_device->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, depth_stencil_view.ReleaseAndGetAddressOf()));

    // TODO: Initialize windows-size dependent objects here
	/// Used during game instantiation for autoscaling system
    current_window_size = DirectX::SimpleMath::Vector2(output_width, output_height);
}

void Engine::Driver::onDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here

    depth_stencil_view.Reset();
    render_target_view.Reset();
    swap_chain.Reset();
    d3d_context.Reset();
    d3d_device.Reset();

    createDevice();

    createResources();
}

GameData* Engine::Driver::getGameData()
{
    return game_data;
}

DrawData* Engine::Driver::getDrawData()
{
    return draw_data;
}
