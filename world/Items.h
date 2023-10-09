#pragma once

#include <string>
#include <vector>

class Player;

class Item
{
public:
    Item(int, std::string const&, std::string const&, std::vector<std::string> const&);

    std::string const& getModelPath() const;
    std::vector<std::string> const& getDescription() const;
    std::string const& getName() const;
    int getId() const&;

    static void onPickup(Item*, Player*);

    static void init();
    static int itemNum;
    static Item* m[128];
    static Item* torchwood;
    static Item* ice;
    static Item* arrow;
    static Item* milk;
    static Item* glass;
    static Item* magician;
    static Item* branch;
    static Item* shoes;
    static Item* kettle;
    static Item* fertilizer;
    static Item* laserbean;
    static Item* potatoseedpack;
    static Item* shield;
    static Item* airbag;
    static Item* sunflower;
protected:
    std::string name;
    std::string modelPath;
    std::vector<std::string> description;
    int id;
};