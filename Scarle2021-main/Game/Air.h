/// Created by Norbert Kupeczki

#pragma once

#include "Ability.h"

class Air :
    public Ability
{
public:
    Air(AbilitiesAir ability);
    ~Air() = default;

private:
    void initAbility(AbilitiesAir ability);
};

