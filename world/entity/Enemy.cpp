#include "Enemy.h"

#include "ModelManager.h"
#include "SoundManager.h"
#include "world/World.h"
#include "Bullet.h"
#include "Game.h"
#include "EnemyBullet.h"

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Enemy::Enemy(World* world, glm::vec3 const& pos) : Entity(world, pos, {{0, 0, 0}, {1, 1, 1}})
{
    this->aiTick = 5;
    this->following = pos;

    this->speed = 1.5f;
}

bool Enemy::isEnemy()
{
    return true;
}

void Enemy::tick()
{
    if (this->aiTick == 0)
    {
        this->aiTick = 5;
        this->updateAi();
    }

    this->aiTick--;

    glm::vec3 ppos = following - pos;
    if (glm::length(ppos) < this->speed * 0.05f && !path.empty())
    {
        auto p = path.back();
        this->following = { p.x + 0.5f, 0, p.y + 0.5f };
        path.pop_back();
        ppos = this->following - pos;
    }
    if (glm::length(ppos) >= this->speed * 0.05f)
        this->setVelocity(glm::normalize(ppos) * this->speed);
    else
    {
        this->setVelocity(ppos / 0.05f / this->speed);
    }

    Player* player = this->world->getPlayer();
    if (this->getAABB().collideWith(player->getAABB()))
    {
        player->hurt(0.5f);
    }
    Entity::tick();
    this->setRotation(glm::atan(this->velocity.x, this->velocity.z));
    this->prevRot = this->rotation;
}

void Enemy::updateAi()
{
    Player* player = this->world->getPlayer();
    glm::ivec2 st = { glm::floor(pos.x), glm::floor(pos.z)};
    glm::ivec2 ed = { glm::floor(player->getPos().x), glm::floor(player->getPos().z) };
    path.clear();
    world->getRoom()->getPathfinder()->findPath(st, ed, path);
    if (!path.empty())
        path.pop_back();
}

void Enemy::onDeath()
{
    world->addScore(100);
}