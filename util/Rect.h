#pragma once

#include <glm/gtc/type_ptr.hpp>

class Rect
{
public:
    glm::ivec2 lb, rt;

    Rect(glm::ivec2 const&, glm::ivec2 const&);
    bool inside(glm::ivec2 const&) const;
};
