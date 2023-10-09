#include "Direction.h"


glm::vec3 Direction::vec[4] =
{
    {0.0f, 0.0f, -1.0f},
    {-1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f},
    {1.0f, 0.0f, 0.0f}
};


glm::ivec2 Direction::ivec[8] =
{
    {0, -1},
    {0, 1},
    {1, 0},
    {-1, 0},
    {-1, -1},
    {-1, 1},
    {1, -1},
    {1, 1}
};

glm::vec3 const& Direction::get3D(int dir)
{
    dir %= 4;
    if (dir < 0) dir += 4;
    return vec[dir];
}

glm::ivec2 const& Direction::get2Di(int dir)
{
    return ivec[dir];
}
