// Created by Ben on 28 / 02 / 2022

#include "pch.h"

// Header
#include "MoreMath.h"

// I couldn't make USE_MATH_DEFINES work, probably just missing something obvious...
#define PI 3.1415926F

float HelperMethods::MoreMath::rotateTowards(DirectX::SimpleMath::Vector2 current, DirectX::SimpleMath::Vector2 target)
{
	float dx = target.x - current.x;
	float dy = target.x - current.x;

	return std::atan2(dy, dx);
}

int HelperMethods::MoreMath::randomRange(int min, int max)
{
	// random seeded in main
	return min + rand() % ((max + 1) - min);
}

float HelperMethods::MoreMath::rad2Degree(float rad)
{
	return rad * (180.0F / PI);
}
