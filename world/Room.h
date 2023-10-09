#pragma once

#include "rendering/Model.h"
#include "entity/Entity.h"

#include <vector>
#include <random>
#include "particle/Particle.h"
#include "util/Pathfinder.h"

class World;

class Room
{
public:
    friend class RoomFactory;

    Room(World*, int, int);
    ~Room();

    void render(const Shader*);
    bool checkCollision(AABB const&) const;
    Entity* hitEnermy(AABB const&) const;
    void tick();
    void addEntity(Entity*);
    void setBlock(int, int, int);
    int getBlock(int, int) const;
    void open(int);
    void setConnect(int, Room*);
    void getEnemiesWithin(std::vector<Entity*>&, AABB const&);
    void spawnEntity(int, int, float, float);
    void onComplete();
    void addParticle(glm::vec3 const&, glm::vec3 const&, glm::vec3 const&);
    void addParticle(glm::vec3 const&, glm::vec3 const&, glm::vec3 const&, int);
    bool isBossRoom() const;
    bool isItemRoom() const;
    bool isShopRoom() const;
    Item* getNearestItem() const;
    Entity* findNearestEnemy() const;
    glm::ivec2 findSpace(int, int);
    std::vector<Entity*> const& getEntities();
    Pathfinder* getPathfinder() const;
protected:
    int block[15][15];
    World* world;
    const Model* blockModels[16];
    Room* next[4];
    int door[4];
    std::vector<AABB> aabbs;
    std::vector<Entity*> entities;
    std::vector<Particle*> particles;
    std::vector<std::pair<glm::vec3, int>> decorations;
    std::random_device rd;
    std::mt19937 rng;
    std::uniform_int_distribution<int> heartDrop;
    std::uniform_int_distribution<int> itemUni;
    int roomFlag;
    Pathfinder* pathfinder;
    int completeFlag;

    static int dx[4];
    static int dy[4];
};