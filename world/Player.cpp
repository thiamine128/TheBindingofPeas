#include "Player.h"

#include "Game.h"
#include "ModelManager.h"
#include "entity/Bullet.h"
#include "SoundManager.h"
#include "util/Direction.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
Player::Player(World* world) : Entity(world, glm::vec3(0.0), AABB({0, 0, 0}, {0.8, 2, 0.8}))
{

    this->model = ModelManager::getModel("assets/models/peashooter.obj");

    this->shootSpeed = 10;
    this->shotCooldown = 0;
    this->health = 3.0f;
    this->hearts = 6;
    this->speed = 3.0f;
    this->immuneTicks = 0;
    this->atk = 1.0;
    this->range = 1.0f;
    this->potatoes = 2;
    this->coins = 15;
    this->face = 0;
    this->shields = 0;
    itemFlag.reset();
}

Player::~Player()
{
}

void Player::shoot(int dir)
{
    if (this->world->shootEnabled()) {
        if (this->shotCooldown <= 0)
        {
            if (itemFlag[Item::branch->getId()])
            {
                for (int i = -1; i <= 1; ++i)
                {
                    auto bullet = new Bullet(this->world, this->pos + glm::vec3(0.0, 0.7, 0.0), this->velocity * 0.2f + 4.0f * glm::normalize(Direction::get3D(dir) + 0.5f * Direction::get3D(dir + i)), 0.3f);
                    Bullet::apply(bullet, this, this->items);
                    this->world->getRoom()->addEntity(bullet);
                    this->shotCooldown = this->shootSpeed;
                }
            }
            else
            {
                auto ve = this->velocity * 0.2f;
                if (dir != -1)
                {
                    ve += 4.0f * Direction::get3D(dir);
                }
                else
                {
                    ve += 4.0f * glm::vec3(-glm::sin(this->rotation), 0.0, -glm::cos(this->rotation));
                }
                auto bullet = new Bullet(this->world, this->pos + glm::vec3(0.0, 0.7, 0.0), ve, 0.3f);
                Bullet::apply(bullet, this, this->items);
                this->world->getRoom()->addEntity(bullet);
                this->shotCooldown = this->shootSpeed;
            }
            SoundManager::play("assets/sounds/shoot.wav");
        }
    }
    if (!Game::getInstance()->getController()->useMouseControl())
    {
        this->face = dir;
        this->setRotation(glm::radians(90.0f * dir));
    }
}

void Player::render(Shader const* shader) const
{
    Entity::render(shader);
    for (int i = 0; i < shields; ++i)
    {
        glm::mat4 model = glm::mat4(1.0);
        model = glm::translate(model, getPosFrame() + Direction::get3D(face + i) * 1.2f);
        model = glm::rotate(model, this->face * glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        shader->setMat4("model", model);
        ModelManager::getModel("assets/models/shield.obj")->render(shader);
    }
}

void Player::tick()
{
    Controller* controller = Game::getInstance()->getController();
    this->velocity = controller->getVelocity() * this->speed;
    Entity::tick();
    if (shotCooldown > 0) --shotCooldown;
    if (Game::getInstance()->getController()->useMouseControl())
    {
        glm::vec3 mousePos = Game::getInstance()->getWorldRenderer()->getWorldPos(glm::vec2(Game::getInstance()->getController()->getMouseX(), Game::getInstance()->getController()->getMouseY()));
        mousePos -= this->pos;
        this->setRotation(-glm::atan(mousePos.z, mousePos.x) - glm::radians(90.0f));
        this->face = glm::floor((this->rotation + glm::radians(405.0f)) / glm::radians(90.0f));
        face %= 4;
        if (controller->isMouseButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
        {
            this->shoot(-1);
        }
    } else if (controller->isKeyPressed(GLFW_KEY_LEFT))
    {
        this->shoot(3);
    } else if (controller->isKeyPressed(GLFW_KEY_RIGHT))
    {
        this->shoot(1);
    } else if (controller->isKeyPressed(GLFW_KEY_UP))
    {
        this->shoot(2);
    } else if (controller->isKeyPressed(GLFW_KEY_DOWN))
    {
        this->shoot(0);
    }
    if (this->immuneTicks > 0)
    {
        this->immuneTicks --;
    }
}

void Player::gainItem(Item* item)
{
    this->items.push_back(item);
    this->itemFlag[item->getId()] = 1;
    Item::onPickup(item, this);
}

int Player::getHearts()
{
     return hearts;
}

int Player::getHealth()
{
    return this->health * 2;
}

double Player::getAtk() const
{
    return this->atk;
}

float Player::getRange() const
{
    return this->range;
}

int Player::getPotatoes() const
{
    return this->potatoes;
}

void Player::addAtk(double v)
{
    this->atk += v;
}

void Player::addRange(float v)
{
    this->range += v;
}

void Player::addShootSpeed(int ticks)
{
    this->shootSpeed -= ticks;
}

void Player::addSpeed(float v)
{
    this->speed += v;
}

void Player::hurt(double d)
{
    if (this->immuneTicks <= 0)
    {
        this->health -= d;
        this->immuneTicks = 40;
        if (hasItem(Item::airbag))
        {
            for (int i = 0; i < 8; ++i)
            {
                float rad = glm::pi<float>() * 2.0 * i / 8.0;
                auto bullet = new Bullet(this->world, this->pos + glm::vec3(0.0, 0.7, 0.0), glm::vec3(glm::sin(rad), 0.0, glm::cos(rad)) * 2.0f, 0.3f);
                Bullet::apply(bullet, this, this->items);
                this->world->getRoom()->addEntity(bullet);
            }
        }
    }
}

void Player::heal()
{
    if (this->getHealth() + 1 <= this->getHearts())
    {
        this->health += 0.5;
    }
}

void Player::addHeart(int v)
{
    hearts += v;
    health += 0.5 * v;
}

void Player::placeMine()
{
    if (this->potatoes)
    {
        world->getRoom()->spawnEntity(9, 0, pos.x, pos.z);
        this->potatoes--;
    }
}

void Player::gainPotato(int v)
{
    this->potatoes += v;
}

void Player::gainCoins(int v)
{
    this->coins += v;
}

int Player::getCoins() const
{
    return this->coins;
}

bool Player::hasItem(Item* item) const
{
    return itemFlag[item->getId()];
}

AABB Player::getShield(int face) const
{
    face = this->face + face;
    face %= 4;
    return AABB(pos + Direction::get3D(face) * 0.9f + Direction::get3D(face + 1) * 0.6f, pos + Direction::get3D(face) * 1.1f + Direction::get3D(face - 1) * 0.6f + glm::vec3(0.0f, 2.0f, 0.0f));
}

int Player::getShields() const
{
    return this->shields;
}

void Player::addShield()
{
    if (this->shields < 4) this->shields++;
}
