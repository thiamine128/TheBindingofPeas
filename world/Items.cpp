#include "Items.h"

#include "World.h"
#include "Player.h"

Item* Item::m[128];
int Item::itemNum = 15;
Item* Item::torchwood = nullptr;
Item* Item::ice = nullptr;
Item* Item::arrow = nullptr;
Item* Item::milk = nullptr;
Item* Item::glass = nullptr;
Item* Item::magician = nullptr;
Item* Item::branch = nullptr;
Item* Item::shoes = nullptr;
Item* Item::kettle = nullptr;
Item* Item::fertilizer = nullptr;
Item* Item::laserbean = nullptr;
Item* Item::potatoseedpack = nullptr;
Item* Item::shield = nullptr;
Item* Item::airbag = nullptr;
Item* Item::sunflower = nullptr;

Item::Item(int id, std::string const& name, std::string const& modelPath, std::vector<std::string> const& description) : name(name), modelPath(modelPath), description(description)
{
    m[id] = this;
    this->id = id;
}

std::string const &Item::getModelPath() const
{
    return this->modelPath;
}

std::vector<std::string> const& Item::getDescription() const
{
    return this->description;
}

std::string const &Item::getName() const
{
    return this->name;
}

int Item::getId() const&
{
    return id;
}

//拾取道具
void Item::onPickup(Item* item, Player* player)
{
    if (item == torchwood)
    {
        player->addAtk(0.5);
    } else if (item == ice)
    {
        player->addAtk(0.5);
    } else if (item == milk)
    {
        player->addShootSpeed(4);
    } else if (item == glass)
    {
        player->addRange(2.0f);
    } else if (item == shoes)
    {
        player->addSpeed(1.0f);
    } else if (item == kettle)
    {
        player->addHeart(4);
    } else if (item == fertilizer)
    {
        player->addAtk(1);
    } else if (item == laserbean)
    {
        player->addShootSpeed(128);
    }
    else if (item == potatoseedpack)
    {
        player->gainPotato(64);
    }
    else if (item == shield)
    {
        player->addShield();
    }

    player->getWorld()->addScore(1000);
}

//初始化道具
void Item::init()
{
    torchwood = new Item(1, "Torchwood", "assets/models/torchwood.obj", {"Explosive pea", "Damage up"});
    ice = new Item(2, "Ice", "assets/models/ice.obj", {"Freezing pea", "Damage up"});
    arrow = new Item(3, "Arrow", "assets/models/peaarrow.obj", {"Piercing pea"});
    milk = new Item(4, "Milk", "assets/models/milk.obj", {"Shoot speed up"});
    glass = new Item(5, "Glass", "assets/models/glass.obj", {"Range up"});
    magician = new Item(6, "Magician", "assets/models/magician.obj", {"Tracking shot"});
    branch = new Item(7, "Branch", "assets/models/branch.obj", {"Triple shot"});
    shoes = new Item(8, "Shoes", "assets/models/shoes.obj", {"Speed up"});
    kettle = new Item(9, "Kettle", "assets/models/kettle.obj", {"Health up"});
    fertilizer = new Item(10, "Fertilizer", "assets/models/fertilizer.obj", {"Pea size up", "Damage up"});
    laserbean = new Item(11, "Laser Bean", "assets/models/laserbean.obj", {"Laser pea"});
    potatoseedpack = new Item(12, "Potato Seed Pack", "assets/models/potatoseedpack.obj", { "Potato mines x 64" });
    shield = new Item(13, "Shield", "assets/models/shield.obj", { "A shield blocking enemies' bullets" });
    airbag = new Item(14, "Airbag", "assets/models/airbag.obj", { "Shoot peas when you get hurt" });
    sunflower = new Item(15, "Sunflower", "assets/models/sunflower.obj", { "More heart drops" });
}
