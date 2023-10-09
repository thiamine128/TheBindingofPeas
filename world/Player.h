#pragma once

#include "Items.h"
#include "entity/Entity.h"
#include "rendering/Model.h"
#include "util/AABB.h"
#include <glm/gtc/type_ptr.hpp>
#include <bitset>

class Player : public Entity
{
public:
    Player(World*);
    ~Player();

    void shoot(int);
    virtual void render(Shader const*) const;
    virtual void tick();
    void gainItem(Item*);
    int getHearts();
    int getHealth();
    double getAtk() const;
    float getRange() const;
    int getPotatoes() const;
    void addAtk(double);
    void addRange(float);
    void addShootSpeed(int);
    void addSpeed(float);
    void hurt(double);
    void heal();
    void addHeart(int);
    void placeMine();
    void gainPotato(int);
    void gainCoins(int);
    int getCoins() const;
    bool hasItem(Item*) const;
    AABB getShield(int) const;
    int getShields() const;
    void addShield();
protected:
    int shotCooldown;
    int immuneTicks;
    int shootSpeed;
    double atk;
    int hearts;
    int shields;
    float range;
    int potatoes;
    int coins;
    int face;
    std::bitset<128> itemFlag;
    std::vector<Item*> items;
};