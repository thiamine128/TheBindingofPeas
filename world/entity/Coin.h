#pragma once

#include "Entity.h"

class Coin : public Entity
{
public:
    Coin(World* world, glm::vec3 const&);

    virtual void tick();
};