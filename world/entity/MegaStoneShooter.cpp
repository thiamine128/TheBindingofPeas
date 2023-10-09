#include "MegaStoneShooter.h"

#include "ModelManager.h"
#include "EnemyBullet.h"
#include "world/World.h"

#include <iostream>

MegaStoneShooter::MegaStoneShooter(World* world, glm::vec3 const& pos, int dir) : StoneShooter(world, pos, dir)
{
	this->model = ModelManager::getModel("assets/models/megastoneshooter.obj");
    this->shootDuration = 0;
}

void MegaStoneShooter::tick()
{
    if (this->shootTick == 1)
    {
        this->shootDuration = 40;
    }
    if (this->shootTick)
    {
        this->shootTick--;
    }
    if (this->shootDuration)
    {
        glm::vec3 v[] =
        {
            {0, 0, 1},
            {1, 0, 0},
            {0, 0, -1},
            {-1, 0, 0}
        };
        auto eb = new EnemyBullet(world, pos + v[dir] * 1.0f + glm::vec3(0.0, 0.2, 0.0), v[dir] * 6.0f, 0.8f, 24.0f, glm::vec4(1.0, 0.4, 0.0, 1.0));
        world->getRoom()->addEntity(eb);
        this->shootDuration--;
    }
    else if (this->shootTick == 0)
    {
        this->shootTick = 40;
    }
    Entity::tick();
}