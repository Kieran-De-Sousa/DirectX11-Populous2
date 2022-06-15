#include "pch.h"
#include "TileTerrain.h"

void TileTerrain::setParams(bool isWater, string _textures)
{
	isWater_ = isWater;
	texture_ = _textures;
}

string TileTerrain::loadTexture()
{
	return texture_;
}
