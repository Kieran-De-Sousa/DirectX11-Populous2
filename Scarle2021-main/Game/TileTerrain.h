#pragma once
#include "RenderSystem.h"
#include "Driver.h"
#include "Coordinator.h"

class TileTerrain
{
public:
    void setParams(bool isWater, string _textures);
    string loadTexture();
    bool isWater() const { return isWater_; }

private:
    bool isWater_;
    string texture_;
};

