/// Created by Norbert Kupeczki

#pragma once
#include "Ability.h"
class Vegetation :
    public Ability
{
public:
    Vegetation(AbilitiesVegetation ability);
    ~Vegetation() = default;

private:
    void initAbility(AbilitiesVegetation ability);
};

