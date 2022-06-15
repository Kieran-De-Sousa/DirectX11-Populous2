/// Created by Norbert Kupeczki

#pragma once

#include "Ability.h"

class Water :
    public Ability
{
public:
    Water(AbilitiesWater ability);
    ~Water() = default;

private:
    void initAbility(AbilitiesWater ability);
};

