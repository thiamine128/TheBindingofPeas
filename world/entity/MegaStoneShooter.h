#pragma once
#include "StoneShooter.h"
class MegaStoneShooter :
    public StoneShooter
{
public:
    MegaStoneShooter(World*, glm::vec3 const&, int);

    virtual void tick();
protected:
    int shootDuration;
};

