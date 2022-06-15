// Created by Ben 15/03/2022

#pragma once

#include <Keyboard.h>

struct KeyboardData
{
	DirectX::Keyboard::State key;
	
	enum class State { PRESSED, HELD, RELEASED };
	State state;
};
