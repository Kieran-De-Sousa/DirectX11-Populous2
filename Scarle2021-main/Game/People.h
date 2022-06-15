/// Created by Norbert Kupeczki

#pragma once

#include "Ability.h"

class People :
    public Ability
{
public:
    People(AbilitiesPeople ability);
    ~People() = default;

private:
    void initAbility(AbilitiesPeople ability);
};

