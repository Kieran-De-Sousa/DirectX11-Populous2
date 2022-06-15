/// Created by Norbert Kupeczki

#pragma once

#include "Ability.h"

class Fire :
    public Ability
{
public:
    Fire(AbilitiesFire ability);
    ~Fire() = default;

private:
    void initAbility(AbilitiesFire ability);
};

