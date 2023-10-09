#pragma once

#include <vector>
#include <glm/gtc/type_ptr.hpp>

class Room;
class Pathfinder
{
public:
    class Node
    {
    public:
        glm::ivec2 pos;
        Node* parent;
        float g, h;

        Node(glm::ivec2 const&, Node*);
    };
    Pathfinder(Room*);

    void findPath(glm::ivec2 const&, glm::ivec2 const&, std::vector<glm::vec2>&);
    Node* findNode(std::vector<Node*> const&, glm::ivec2) const;
    float dist(glm::vec2 const&, glm::vec2 const&);
protected:
    Room* room;
};