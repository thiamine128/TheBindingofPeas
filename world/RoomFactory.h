#pragma once

#include "Room.h"

class RoomFactory
{
public:
	static Room* createByLayout(World*, int, int, std::string const&);
	static void generateDecoration(Room*);
	static void spawnEntityInRoom(Room*, int, int, float, float);
	static void setupBlockModels(Room*, int);
};

