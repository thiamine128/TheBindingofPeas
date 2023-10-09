#include "Bullet.h"

#include "ModelManager.h"
#include "SoundManager.h"
#include "world/World.h"
#include <iostream>

//玩家子弹
Bullet::Bullet(World* world, glm::vec3 const& pos, glm::vec3 const& velocity, float size) : Entity(world, pos, {{0, 0, 0}, {size, size, size}}), size(size), uni(-1.0, 1.0)
{
    this->velocity = velocity;
    this->model = ModelManager::getModel("assets/models/sphere.obj");
    this->fire = ModelManager::getModel("assets/models/fire.obj");
    this->damage = 1.0f;
    this->flag.reset();
    this->range = 1.0f;
    this->color = glm::vec4(0.64, 1.0, 0.15, 1.0);
    this->initialPos = pos;
    this->totDist = 0;
}

void Bullet::render(const Shader* shader) const
{
    glm::mat4 model = glm::mat4(1.0f);
    
    model = glm::translate(model, this->getPosFrame());
    model = glm::scale(model, glm::vec3(size));
    model = glm::rotate(model, this->rotation, glm::vec3(0.0, 1.0, 0.0));
    
    shader->setMat4("model", model);
    
    shader->setVec4("color", color);
    this->model->render(shader);
    shader->setVec4("color", glm::vec4(1.0));
    if (this->isFire())
    {
        glm::vec4 fireColor = glm::vec4(1.0, 0.64, 0.28, 1.0);
        if (this->isFrozen())
        {
            fireColor = glm::vec4(0.0, 1.0, 1.0, 1.0);
        }
        shader->setVec4("color", fireColor);
        this->fire->render(shader);
        shader->setVec4("color", glm::vec4(1.0));
    }
}

void Bullet::tick()
{
    float horizontalV = glm::length(glm::vec3(velocity.x, 0, velocity.z));
    if ((!isLaser() && totDist > range))
    {
        this->velocity.y -= 0.15;
    }
    if (this->isTracking())
    {
        Entity* ent = world->getRoom()->findNearestEnemy();
        if (ent != nullptr)
        {
            auto v = ent->getPos() - pos;
                v.y = 0.0f;
                v = glm::normalize(v);
            this->velocity.x = v.x * horizontalV;
            this->velocity.z = v.z * horizontalV;
        }

        world->getRoom()->addParticle(pos + glm::vec3(uni(rng), uni(rng), uni(rng)) * size * 0.5f, glm::vec3(0.0), glm::vec3(0.6, 0.0, 0.8), 8);
    }
    Entity::tick();
    this->totDist += horizontalV * 0.05;
    Entity* hit = this->world->getRoom()->hitEnermy(this->getAABB());
    if (hit != nullptr)
    {
        if (!this->isPierce())
            this->onCollide(0);
        this->onHit(hit);
    }
    if (!shouldRemove())
    {
        if (glm::abs(pos.x) > 7.0f || glm::abs(pos.z) > 7.0f)
        {
            this->onCollide(0);
        }
    }
    this->setRotation(glm::atan(this->velocity.x, this->velocity.z));
}

void Bullet::onCollide(int)
{
    this->remove();
}

void Bullet::onRemove()
{
    glm::vec3 pcolor = color;
    if (this->isFire())
    {
        pcolor = glm::vec3(1.0, 0.64, 0.28);
    }
    if (this->isFrozen())
    {
        pcolor = glm::vec3(0.0, 1.0, 1.0);
    }
    for (int i = 0; i < 6; ++i)
    {
        this->world->getRoom()->addParticle(this->pos, glm::vec3(uni(rng), uni(rng), uni(rng)), pcolor);
    }
}

void Bullet::onHit(Entity* hit)
{
    if (this->isFire())
    {
        Room* room = hit->getWorld()->getRoom();
        std::vector<Entity*> entities;
        room->getEnemiesWithin(entities, this->getAABB().inflate(glm::vec3(1.0f)));
        for (auto e : entities)
        {
            this->doDamage(hit);
            if (this->isFrozen())
                e->freeze(60);
        }
    } else if (this->isFrozen())
    {
        hit->freeze(60);
        this->doDamage(hit);
    } else {
        this->doDamage(hit);
    }
}

void Bullet::setFire()
{
    this->flag[0] = 1;
}

void Bullet::setFrozen()
{
    this->flag[1] = 1;
}

void Bullet::setPierce()
{
    this->flag[2] = 1;
    this->hasCollision = false;
}

void Bullet::setTracking()
{
    this->flag[3] = 1;
}

void Bullet::setLaser()
{
    this->flag[4] = 1;
}

void Bullet::addDamage(float damage)
{
    this->damage += damage;
}

bool Bullet::isFire() const
{
    return this->flag[0];
}

bool Bullet::isFrozen() const
{
    return this->flag[1];
}

bool Bullet::isPierce() const
{
    return this->flag[2];
}

bool Bullet::isTracking() const
{
    return this->flag[3];
}

bool Bullet::isLaser() const
{
    return this->flag[4];
}

void Bullet::doDamage(Entity* e)
{
    if (!this->isPierce())
    {
        e->hurt(this->damage);
        SoundManager::play("assets/sounds/hit.mp3");
    }
    else
    {
        if (pierced.count(e) == 0)
        {
            SoundManager::play("assets/sounds/hit.mp3");
            e->hurt(this->damage);
            pierced.insert(e);
        }
    }
}

void Bullet::apply(Bullet* bullet, Player* player, std::vector<Item*> const& items)
{
    bullet->damage = player->getAtk();
    bullet->range = player->getRange();
    for (auto item : items)
    {
        if (item == Item::torchwood)
        {
            bullet->setFire();
        } else if (item == Item::ice)
        {
            bullet->setFrozen();
        } else if (item == Item::arrow)
        {
            bullet->setPierce();
        } else if (item == Item::magician)
        {
            bullet->setTracking();
        } else if (item == Item::fertilizer)
        {
            bullet->size += 0.3;
        } else if (item == Item::laserbean)
        {
            bullet->setLaser();
            bullet->speed = 4.0f;
        }
    }

    if (bullet->isFire())
    {
        bullet->color = glm::vec4(0.0, 0.0, 0.0, 1.0);
    } else if (bullet->isFrozen())
    {
        bullet->color = glm::vec4(0.12, 0.64, 0.68, 1.0);
    }

    if (bullet->isPierce())
    {
        bullet->model = ModelManager::getModel("assets/models/peaarrow.obj");
    }
    if (bullet->isLaser())
    {
        bullet->damage *= 0.1;
        bullet->model = ModelManager::getModel("assets/models/laser.obj");
    }
}