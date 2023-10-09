#include "PlasticBag.h"

#include "ModelManager.h"
#include "world/World.h"
#include "EnemyBullet.h"

PlasticBag::PlasticBag(World* world, glm::vec3 const& pos) : Enemy(world, pos)
{
    this->model = ModelManager::getModel("assets/models/plasticbag.obj");
    this->health = 240;
    this->scale = glm::vec3(3.0);
    this->aabb = { {0, 0, 0}, {3, 3, 3} };
}

void PlasticBag::tick()
{
    Entity::tick();

    glm::vec3 v[] =
    {
        {1, 0, 0},
        {0, 0, 1},
        {-1, 0, 0},
        {0, 0, -1}
    };
    shotRot += 0.01;
    glm::mat4 rotMatrix = glm::mat4(1.0);
    rotMatrix = glm::rotate(rotMatrix, shotRot, glm::vec3(0.0, 1.0, 0.0));
    for (int i = 0; i < 4; ++i)
    {
        glm::vec3 velocity = rotMatrix * glm::vec4(v[i], 0.0);
        auto eb = new EnemyBullet(world, pos + glm::vec3(0.0, 1.0, 0.0), velocity * 2.0f, 1.0f, 12.0f, glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
        world->getRoom()->addEntity(eb);
    }
}

void PlasticBag::onDeath()
{
    this->world->addScore(1200);
    this->world->finish();
}
