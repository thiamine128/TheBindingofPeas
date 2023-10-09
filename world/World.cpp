#include "World.h"

#include "Game.h"
#include "LayerGenerator.h"
#include "SoundManager.h"

#include <iostream>

//背景音乐路径
std::string World::music[5] =
{
    "",
    "assets/music/the caves.ogg",
    "assets/music/kave diluvii (flooded caves) loop.ogg",
    "assets/music/library.ogg",
    "assets/music/the calm.ogg"
};

//创建世界
World::World(Challenge* challenge)
{
    this->player = new Player(this);
    this->stage = 0;
    this->nextStage();
    this->score = 10000;
    this->scoreTick = 20;
    this->newRec = false;
    this->finished = false;
    this->challenge = challenge;
    this->enableShoot = true;
    if (this->challenge)
        this->challenge->onStart(this);
}

//销毁占用内存
World::~World()
{
    delete this->player;
    delete this->challenge;
    LayerGenerator::clear();
    SoundManager::stopLoop();
}

Player *World::getPlayer() const
{
    return this->player;
}

Room *World::getRoom() const
{
    return this->room;
}

Room *World::getRoom(int x, int y) const
{
    return grid[x][y];
}

void World::setRoom(Room* room)
{
    this->room = room;
}

//世界逻辑更新
void World::tick()
{
    //更新玩家
    this->player->tick();
    //更新房间
    this->room->tick();
    
    //分数随时间减少
    if (this->scoreTick == 0 && !finished)
        this->score--, this->scoreTick = 20;
    else
        this->scoreTick--;

    //进入下一层
    if (this->prepareNext)
    {
        this->nextStage();
    }
}

//进入下一层
void World::nextStage()
{
    this->stage++;
    LayerGenerator::clear();
    this->prepareNext = false;
    this->room = LayerGenerator::generateRooms(this, 10, this->stage);
    for (int i = 0; i < 13; ++i)
    {
        for (int j = 0; j < 13; ++j)
        {
            this->grid[i][j] = LayerGenerator::m[i][j];
        }
    }
    if (this->stage != 1)
        SoundManager::stopLoop();
    SoundManager::playLoop(music[this->stage]);
}

//准备进入下一层
void World::prepareNextStage()
{
    this->prepareNext = true;
}

//分数相关
void World::addScore(int v)
{
    this->score += v;
}

int World::getScore() const
{
    return this->score;
}

bool World::shootEnabled() const
{
    return this->enableShoot;
}

void World::disableShoot()
{
    this->enableShoot = false;
}

//退出游戏时资源释放
void World::finish()
{
    this->finished = true;
    newRec =  Game::getInstance()->updateScore(score);
    Game::getInstance()->getScreenManager()->pushWinScreen();
}
