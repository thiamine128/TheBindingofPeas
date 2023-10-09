#pragma once

#include "Entity.h"
#include "rendering/Model.h"

#include <vector>
#include <random>


class Enemy : public Entity
{
public:
    Enemy(World*, glm::vec3 const&);
    virtual bool isEnemy();
    virtual void tick();
    virtual void updateAi();
    virtual void onDeath();
protected:
    int aiTick;
    glm::vec3 following;
    std::vector<glm::vec2> path;
};