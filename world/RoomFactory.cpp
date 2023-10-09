#include "RoomFactory.h"

#include "ModelManager.h"
#include "entity/Zombie.h"
#include "entity/ZombieShooter.h"
#include "entity/ItemEntity.h"
#include "entity/HeartEntity.h"
#include "entity/EntranceEntity.h"
#include "entity/StoneShooter.h"
#include "entity/PotatoMine.h"
#include "entity/BabyPlum.h"
#include "entity/SlimeBoss.h"
#include "entity/Dragon.h"
#include "entity/Coin.h"
#include "entity/Good.h"
#include "entity/PlasticBag.h"
#include "entity/MegaStoneShooter.h"

#include <fstream>
#include <sstream>

//根据布局文件创建房间
Room* RoomFactory::createByLayout(World* world, int stage, int roomFlag, std::string const& layout)
{
    Room* room = new Room(world, stage, roomFlag);
    for (int i = -7; i <= 7; ++i)
    {
        for (int j = -7; j <= 7; ++j)
        {
            if (i == -7 || i == 7 || j == -7 || j == 7)
            {
                room->setBlock(i, j, 1);
            }
            else
            {
                room->setBlock(i, j, 0);
            }
        }
    }
    std::ifstream f;
    f.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    f.open(layout);

    std::stringstream fs;
    fs << f.rdbuf();

    f.close();

    int b;
    for (int i = -6; i <= 6; ++i)
    {
        for (int j = -6; j <= 6; ++j)
        {
            fs >> b;
            room->setBlock(i, j, b);
        }
    }

    int t, d;
    float x, y;
    char s;
    while (fs >> t)
    {
        fs >> s >> d >> x >> y;
        room->spawnEntity(t, d, x, y);
    }
    setupBlockModels(room, stage);
    generateDecoration(room);
    return room;
}

void RoomFactory::generateDecoration(Room* room)
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> decId(1, 4);
    std::uniform_real_distribution<float> pos(-6.0f, 6.0f);
    std::uniform_int_distribution<int> decNum(4, 10);

    int num = decNum(rng);
    for (int i = 0; i < num; ++i)
    {
        room->decorations.push_back({ { pos(rng), 0.0f, pos(rng) }, decId(rng) });
    }
}

//在房间中生成实体
void RoomFactory::spawnEntityInRoom(Room* room, int id, int ex, float x, float y)
{
    switch (id)
    {
    case 0:
        if (ex == 0)
        {
            ex = room->itemUni(room->rng);
        }
        room->addEntity(new ItemEntity(room->world, glm::vec3(x, 0.0, y), ex));
        break;
    case 1:
        room->addEntity(new Zombie(room->world, glm::vec3(x, 0.0, y), ex));
        break;
    case 2:
        room->addEntity(new SlimeBoss(room->world, glm::vec3(x, 0.0, y)));
        break;
    case 3:
        room->addEntity(new EntranceEntity(room->world, glm::vec3(x, 0.0, y)));
        break;
    case 4:
        room->addEntity(new ZombieShooter(room->world, glm::vec3(x, 0.0, y), ex));
        break;
    case 5:
        room->addEntity(new HeartEntity(room->world, glm::vec3(x, 0.0, y)));
        break;
    case 6:
        room->addEntity(new BabyPlum(room->world, glm::vec3(x, 0.0, y)));
        break;
    case 7:
        room->addEntity(new Dragon(room->world, glm::vec3(x, 0.0, y)));
        break;
    case 8:
        room->addEntity(new StoneShooter(room->world, glm::vec3(x, 0.0, y), ex));
        break;
    case 9:
        room->addEntity(new PotatoMine(room->world, glm::vec3(x, 0.0, y)));
        break;
    case 10:
        room->addEntity(new Coin(room->world, glm::vec3(x, 0.0, y)));
        break;
    case 11:
        if (ex == 0)
        {
            ex = room->itemUni(room->rng);
        }
        room->addEntity(new Good(room->world, glm::vec3(x, 0.0, y), ex));
        break;
    case 12:
        room->addEntity(new PlasticBag(room->world, glm::vec3(x, 0.0, y)));
        break;
    case 13:
        room->addEntity(new MegaStoneShooter(room->world, glm::vec3(x, 0.0, y), ex));
        break;
    default:
        break;
    }
}

//加载方框模型
void RoomFactory::setupBlockModels(Room* room, int stage)
{
    switch (stage)
    {
    case 1:
        room->blockModels[0] = ModelManager::getModel("assets/models/scene.obj");
        room->blockModels[1] = ModelManager::getModel("assets/models/tree.obj");
        room->blockModels[2] = ModelManager::getModel("assets/models/rock.obj");
        room->blockModels[3] = ModelManager::getModel("assets/models/mushroom.obj");
        room->blockModels[4] = ModelManager::getModel("assets/models/stoneshooterblock.obj");
        break;
    case 2:
        room->blockModels[0] = ModelManager::getModel("assets/models/cave.obj");
        room->blockModels[1] = ModelManager::getModel("assets/models/cavewall.obj");
        room->blockModels[2] = ModelManager::getModel("assets/models/rock.obj");
        room->blockModels[3] = ModelManager::getModel("assets/models/mushroom.obj");
        room->blockModels[4] = ModelManager::getModel("assets/models/stoneshooterblock.obj");
        break;
    case 3:
        room->blockModels[0] = ModelManager::getModel("assets/models/street.obj");
        room->blockModels[1] = ModelManager::getModel("assets/models/cone.obj");
        room->blockModels[2] = ModelManager::getModel("assets/models/rock.obj");
        room->blockModels[3] = ModelManager::getModel("assets/models/mushroom.obj");
        room->blockModels[4] = ModelManager::getModel("assets/models/stoneshooterblock.obj");
        break;
    case 4:
        room->blockModels[0] = ModelManager::getModel("assets/models/desert.obj");
        room->blockModels[1] = ModelManager::getModel("assets/models/deadtree.obj");
        room->blockModels[2] = ModelManager::getModel("assets/models/rock.obj");
        room->blockModels[3] = ModelManager::getModel("assets/models/mushroom.obj");
        room->blockModels[4] = ModelManager::getModel("assets/models/stoneshooterblock.obj");
        break;
    default:
        break;
    }
}
