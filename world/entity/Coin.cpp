#include "Coin.h"

#include "world/World.h"
#include "ModelManager.h"
#include "SoundManager.h"

Coin::Coin(World *world, glm::vec3 const& pos) : Entity(world, pos, {{0, 0, 0}, {1, 1, 1}})
{
    this->model = ModelManager::getModel("assets/models/coin.obj");
    this->scale = glm::vec3(0.5);
    SoundManager::play("assets/sounds/dime drop.wav");
}

void Coin::tick()
{
    Entity::tick();

    if (world->getPlayer()->getAABB().collideWith(this->getAABB()))
    {
        this->remove();
        world->getPlayer()->gainCoins(1);
        SoundManager::play("assets/sounds/dime pick up.wav");
    }
}