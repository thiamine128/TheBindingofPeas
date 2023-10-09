#include "Room.h"

#include "rendering/glhelper.h"
#include "ModelManager.h"
#include "entity/Enemy.h"
#include "World.h"
#include "RoomFactory.h"
#include "entity/ItemEntity.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

int Room::dx[4] = {-1, 0, 0, 1};
int Room::dy[4] = {0, -1, 1, 0};

//创建新房间，指定布局
Room::Room(World* world, int stage, int roomFlag) : world(world), rng(rd()), heartDrop(0, 100), roomFlag(roomFlag)
{
    //地面碰撞箱
    this->aabbs.push_back({{-9.0f, 0.0f, -9.0f}, {9.0f, 0.0f, 9.0f}});

    //物品id随机数
    itemUni = std::uniform_int_distribution<int>(1, Item::itemNum);

    //初始化四个方向的门
    for (int i = 0; i < 4; ++i)
        next[i] = nullptr, door[i] = 0;
    this->completeFlag = 0;
    this->pathfinder = new Pathfinder(this);
}

//释放内存
Room::~Room()
{
    for (auto e : entities)
        delete e;
    for (auto p : particles)
        delete p;
}

//房间内的渲染
void Room::render(const Shader* shader)
{
    //渲染地面
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(64.0f));
    shader->setMat4("model", modelMatrix);
    this->blockModels[0]->render(shader);

    //渲染实体
    for (auto e : entities)
    {
        e->render(shader);
    }

    //渲染方块
    for (int i = -7; i <= 7; ++i)
    {
        for (int j = -7; j <= 7; ++j)
        {
            if (this->getBlock(i, j) != 0)
            {
                glm::mat4 modelMat = glm::mat4(1.0);
                modelMat = glm::translate(modelMat, glm::vec3(i + 0.5f, 0, j + 0.5f));
                shader->setMat4("model", modelMat);
                this->blockModels[this->getBlock(i, j)]->render(shader);
            }
        }
    }

    //渲染粒子
    for (auto p : particles)
    {
        glm::vec3 const& pos = p->getPos();
        glm::mat4 modelMat = glm::mat4(1.0);
        modelMat = glm::translate(modelMat, pos);
        modelMat = glm::scale(modelMat, glm::vec3(p->getScale()));
        modelMat = glm::scale(modelMat, glm::vec3(0.3f));
        shader->setMat4("model", modelMat);
        shader->setVec4("color", p->getColor());
        ModelManager::getModel("assets/models/sphere.obj")->render(shader);
        shader->setVec4("color", glm::vec4(1.0));
    }

    for (auto d : decorations)
    {
        glm::mat4 modelMat = glm::mat4(1.0);
        modelMat = glm::translate(modelMat, d.first);
        shader->setMat4("model", modelMat);
        ModelManager::getModel("assets/models/decoration_" + std::to_string(d.second) + ".obj")->render(shader);
    }
}

//房间内碰撞检测
bool Room::checkCollision(AABB const& aabb) const
{
    for (auto e : aabbs) {
        if (e.collideWith(aabb))
            return true;
    }
    for (int i = -7; i <= 7; ++i)
    {
        for (int j = -7; j <= 7; ++j)
        {
            if (this->getBlock(i, j) != 0)
            {
                AABB blockAABB = {{i + 0.1, 0, j + 0.1}, {i + 0.9, 2, j + 0.9}};
                if (blockAABB.collideWith(aabb))
                {
                    return true;
                }
            }
        }
    }
    return false;
}

//获取aabb范围内的一个实体
Entity* Room::hitEnermy(AABB const& aabb) const
{
    for (auto e : entities)
    {
        if (e->isEnemy() && e->getAABB().collideWith(aabb))
        {
            return e;
        }
    }
    return nullptr;
}

//房间逻辑更新
void Room::tick()
{
    bool hasEnermy = false;
    //更新实体
    for (int i = entities.size() - 1; i >= 0; --i)
    {
        entities[i]->tick();
        if (entities[i]->shouldRemove())
        {
            delete entities[i];
            entities.erase(entities.begin() + i);
        } else if (entities[i]->isEnemy())
        {
            hasEnermy = true;
        }
    }
    //更新粒子
    for (int i = particles.size() - 1; i >= 0; --i)
    {
        particles[i]->tick();
        if (!particles[i]->alive())
        {
            delete particles[i];
            particles.erase(particles.begin() + i);
        }
    }
    //房间完成
    if (this->completeFlag == 0 && !hasEnermy)
    {
        this->onComplete();
        this->completeFlag = 1;
    }
    //判断玩家是否进入下一个房间
    glm::vec3 ppos = world->getPlayer()->getPos();
    if (ppos.x < -7.0f)
    {
        this->world->setRoom(next[0]);
        this->world->getPlayer()->setPos(glm::vec3(5.5f, 0.0f, 0.0f));
    } else if (ppos.x > 7.0f)
    {
        this->world->setRoom(next[3]);
        this->world->getPlayer()->setPos(glm::vec3(-5.5f, 0.0f, 0.0f));
    } else if (ppos.z < -7.0f)
    {
        this->world->setRoom(next[1]);
        this->world->getPlayer()->setPos(glm::vec3(0.0f, 0.0f, 5.5f));
    } else if (ppos.z > 7.0f)
    {
        this->world->setRoom(next[2]);
        this->world->getPlayer()->setPos(glm::vec3(0.0f, 0.0f, -5.5f));
    }
}

void Room::addEntity(Entity* entity)
{
    this->entities.push_back(entity);
}

void Room::setBlock(int x, int y, int b)
{
    this->block[x + 7][y + 7] = b;
}

int Room::getBlock(int x, int y) const
{
    return this->block[x + 7][y + 7];
}

//打开房间的连接口
void Room::open(int dir)
{
    door[dir] = 1;
    if (dx[dir] == 0)
        for (int i = -1; i <= 1; ++i)
            this->setBlock(i, 7 * dy[dir], 3);
    if (dy[dir] == 0)
        for (int i = -1; i <= 1; ++i)
            this->setBlock(7 * dx[dir], i, 3);
}

void Room::setConnect(int dir, Room* room)
{
    this->next[dir] = room;
}

//获取范围内的所有实体
void Room::getEnemiesWithin(std::vector<Entity*>& entities, AABB const& aabb)
{
    for (auto e : this->entities)
    {
        if (e->getAABB().collideWith(aabb) && e->isEnemy())
        {
            entities.emplace_back(e);
        }
    }
}

//根据参数生成实体
void Room::spawnEntity(int id, int ex, float x, float y)
{
    RoomFactory::spawnEntityInRoom(this, id, ex, x, y);
}

//清理房间事件
void Room::onComplete()
{
    //获得分数并打开门
    world->addScore(1000);
    for (int dir = 0; dir < 4; ++dir)
    {
        if (door[dir])
        {
            if (dx[dir] == 0)
                for (int i = -1; i <= 1; ++i)
                    this->setBlock(i, 7 * dy[dir], 0);
            if (dy[dir] == 0)
                for (int i = -1; i <= 1; ++i)
                    this->setBlock(7 * dx[dir], i, 0);
        }
    }

    //随机掉落资源
    int heartFreq = 60;
    Player* player = world->getPlayer();
    if (player->hasItem(Item::sunflower))
        heartFreq = 100;
    if (heartDrop(rng) < 60)
    {
        
        auto pos = findSpace(floor(player->getPos().x), floor(player->getPos().y));
        this->spawnEntity(5, 0, pos.x + 0.5, pos.y + 0.5);
    }

    if (heartDrop(rng) < 7)
    {
        auto pos = findSpace(floor(player->getPos().x), floor(player->getPos().y));
        this->spawnEntity(0, 0, pos.x + 0.5, pos.y + 0.5);
    }

    if (heartDrop(rng) < 30)
    {
        player->gainPotato(1);
    }

    if (heartDrop(rng) < 50)
    {
        auto pos = findSpace(floor(player->getPos().x), floor(player->getPos().y));
        this->spawnEntity(10, 0, pos.x + 0.5, pos.y + 0.5);
    }
}

void Room::addParticle(glm::vec3 const& pos, glm::vec3 const& velocity, glm::vec3 const& color)
{
    addParticle(pos, velocity, color, 12);
}

void Room::addParticle(glm::vec3 const& pos, glm::vec3 const& velocity, glm::vec3 const& color, int life)
{
    particles.emplace_back(new Particle(pos, velocity, color, life));

}

bool Room::isBossRoom() const
{
    return roomFlag & 1;
}

bool Room::isItemRoom() const
{
    return roomFlag & 2;
}

bool Room::isShopRoom() const
{
    return roomFlag & 4;
}

//获取离玩家最近的物品
Item *Room::getNearestItem() const
{
    Item* ret = nullptr;
    float dist = 100.0f;
    for (auto i : entities)
    {
        if (i->isItem())
        {
            if (glm::length(i->getPos() - world->getPlayer()->getPos()) < dist)
            {
                dist = glm::length(i->getPos() - world->getPlayer()->getPos());
                ret = ((ItemEntity*) i)->getItem();
            }
        }
    }
    return ret;
}

//获取离玩家最近的敌人
Entity *Room::findNearestEnemy() const
{
    double dist = 400.0f;
    Entity* ret = nullptr;
    for (auto e : entities)
    {
        if (e->isEnemy() && glm::length(e->getPos() - world->getPlayer()->getPos()) < dist)
        {
            dist = glm::length(e->getPos() - world->getPlayer()->getPos());
            ret = e;
        }
    }
    return ret;
}

//找到玩家附近的空地
glm::ivec2 Room::findSpace(int x, int y)
{
    int dist = 400;
    glm::ivec2 ret = glm::ivec2(0);
    for (int i = -7; i <= 7; ++i)
    {
        for (int j = -7; j <= 7; ++j)
        {
            if (getBlock(i, j) == 0)
            {
                int dx = i - x, dy = j - y;
                int d = dx * dx + dy * dy;
                if (d >= 5 && d < dist)
                {
                    dist = d;
                    ret = glm::ivec2(i, j);
                }
            }
        }
    }
    return ret;
}

std::vector<Entity*> const& Room::getEntities()
{
    return this->entities;
}

Pathfinder* Room::getPathfinder() const
{
    return this->pathfinder;
}
