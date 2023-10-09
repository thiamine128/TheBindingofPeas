#include "Good.h"

#include "ModelManager.h"
#include "world/World.h"

Good::Good(World* world, glm::vec3 const& pos, int item) : Entity(world, pos, {{0, 0, 0}, {1, 1, 1}}), item(item)
{
    this->model = ModelManager::getModel("assets/models/tag.obj");
}

void Good::tick()
{
    Entity::tick();
    Player* player = world->getPlayer();
    if (this->getAABB().collideWith(player->getAABB()))
    {
        if (player->getCoins() >= 10)
        {
            player->gainCoins(-10);
            player->gainItem(Item::m[item]);
            this->remove();
        }
    }
}

void Good::render(const Shader* shader) const
{
    Entity::render(shader);
    glm::mat4 modelMat = glm::mat4(1.0);
    modelMat = glm::translate(modelMat, this->getPosFrame() + glm::vec3(0.0, 0.0, 0.8));
    modelMat = glm::scale(modelMat, glm::vec3(1.2));
    shader->setMat4("model", modelMat);
    ModelManager::getModel(Item::m[item]->getModelPath())->render(shader);
}

bool Good::isItem() const
{
    return true;
}
