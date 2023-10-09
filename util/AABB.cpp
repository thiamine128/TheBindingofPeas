#include "AABB.h"

AABB::AABB(glm::vec3 v1, glm::vec3 v2)
{
    min.x = glm::min(v1.x, v2.x);
    min.y = glm::min(v1.y, v2.y);
    min.z = glm::min(v1.z, v2.z);
    max.x = glm::max(v1.x, v2.x);
    max.y = glm::max(v1.y, v2.y);
    max.z = glm::max(v1.z, v2.z);
}

bool AABB::collideWith(AABB const& aabb) const
{
    return (this->max.x > aabb.min.x)
        && (this->max.y > aabb.min.y)
        && (this->max.z > aabb.min.z)
        && (this->min.x < aabb.max.x)
        && (this->min.y < aabb.max.y)
        && (this->min.z < aabb.max.z);
}

float AABB::getMinX() const
{
    return this->min.x;
}

float AABB::getMinY() const
{
    return this->min.y;
}

float AABB::getMinZ() const
{
    return this->min.z;
}

float AABB::getMaxX() const
{
    return this->max.x;
}

float AABB::getMaxY() const
{
    return this->max.y;
}

float AABB::getMaxZ() const
{
    return this->max.z;
}

bool AABB::insideXZ(glm::vec3 const& pos) const
{
    return this->min.x <= pos.x
        && this->min.z <= pos.z
        && this->max.x >= pos.x
        && this->max.z >= pos.z;
}

AABB AABB::offset(glm::vec3 const& off) const
{
    return AABB(this->min + off, this->max + off);
}

AABB AABB::inflate(glm::vec3 const& off) const
{
    return AABB(min - off ,max + off);
}
