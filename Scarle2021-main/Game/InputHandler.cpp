// Created by Ben 10/03/2022

#include "pch.h"

#include "InputHandler.h"

Engine::InputHandler::InputHandler(HWND* _window, GameData* _game_data, SceneHandler* _scene_handler)
    : window(_window), game_data(_game_data), scene_handler(_scene_handler)
{
    // Set up keyboard/mouse
    keyboard = std::make_unique<Keyboard>();
    mouse = std::make_unique<Mouse>();
    mouse->SetWindow(*window);
    mouse->SetMode(Mouse::MODE_RELATIVE);

    // Hide the mouse pointer
    ShowCursor(false);
}

void Engine::InputHandler::update(DX::StepTimer const& timer)
{
	pollKeyBoard();
	pollMouse();
}

void Engine::InputHandler::pollKeyBoard()
{
    // Read keyboard data
    game_data->m_KBS = keyboard->GetState();
    game_data->m_KBS_tracker.Update(game_data->m_KBS);

    /* Quit game on hiting escape
    if (game_data->m_KBS.Escape)
    {
        PostQuitMessage(0);
    }
    */

    // Forward the key data
    scene_handler->getCurrentScene()->onKey(game_data->m_KBS_tracker);
}

void Engine::InputHandler::pollMouse()
{
    // Read mouse data
    game_data->m_MS = mouse->GetState();
    game_data->m_MS_tracker.Update(game_data->m_MS);

    // Notify the current scene of the mouse state
    // scene_handler->getCurrentScene()->onMouse(); Kieran should do this I think?? idk how ui

    // Lock the cursor to the centre of the window
    RECT rect_window;
    GetWindowRect(*window, &rect_window);
    SetCursorPos((rect_window.left + rect_window.right) >> 1, (rect_window.bottom + rect_window.top) >> 1);
    
    // Forwarding the mouse data - Norbert 26/03/2022
    scene_handler->getCurrentScene()->onMouse(game_data->m_MS_tracker);
}

// Will make the 'polling' more advanced soontm
