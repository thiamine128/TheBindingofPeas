#pragma once

#include "Enemy.h"

class PlasticBag : public Enemy
{
public:
    PlasticBag(World*, glm::vec3 const&);

    virtual void tick();
    virtual void onDeath();
protected:
    float shotRot;
};