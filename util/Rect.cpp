#include "Rect.h"

Rect::Rect(glm::ivec2 const& lb, glm::ivec2 const& rt) : lb(lb), rt(rt)
{

}

bool Rect::inside(glm::ivec2 const& pos) const
{
    return pos.x >= lb.x && pos.x <= rt.x
        && pos.y >= lb.y && pos.y <= rt.y;
}