/// Created by Norbert Kupeczki

#pragma once
#include "Ability.h"
class Earth :
    public Ability
{
public:
    Earth(AbilitiesEarth ability);
    ~Earth() = default;

private:
    void initAbility(AbilitiesEarth ability);
};

