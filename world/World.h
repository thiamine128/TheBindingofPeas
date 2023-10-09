#pragma once

class Construction;

#include "Player.h"
#include "Room.h"
#include "challenge/Challenge.h"

#include <vector>

#include <glm/gtc/type_ptr.hpp>

#define CHUNK_NUM 16

class World
{
public:
    World(Challenge*);
    ~World();

    Player* getPlayer() const;
    Room* getRoom() const;
    Room* getRoom(int, int) const;
    void setRoom(Room*);
    void tick();
    void nextStage();
    void prepareNextStage();
    void addScore(int);
    int getScore() const;
    bool shootEnabled() const;
    void disableShoot();
    void finish();

    bool newRec;
protected:
    Player* player;
    Room* room;
    Room* grid[13][13];
    int stage;
    int score;
    int scoreTick;
    bool prepareNext;
    bool finished;
    bool enableShoot;
    Challenge* challenge;

    static std::string music[5];
};