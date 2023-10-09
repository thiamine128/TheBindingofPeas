#include "Pathfinder.h"

#include "Direction.h"

#include "world/Room.h"

#include <queue>
#include <map>
#include <unordered_set>
#include <iostream>

Pathfinder::Pathfinder(Room* room) : room(room)
{
}

void Pathfinder::findPath(glm::ivec2 const& st, glm::ivec2 const& ed, std::vector<glm::vec2>& ret)
{
	Node* current = new Node(st, nullptr);
	
	std::vector<Node*> openSet, closedSet;
	openSet.emplace_back(current);
	while (!openSet.empty())
	{
		auto currentit = openSet.begin();
		current = *currentit;
		for (auto it = openSet.begin(); it != openSet.end(); ++it)
		{
			auto node = *it;
			if (node->g + node->h <= current->g + current->h)
			{
				current = node;
				currentit = it;
			}
		}
		if (current->pos == ed)
		{
			break;
		}

		closedSet.push_back(current);
		openSet.erase(currentit);

		for (int i = 0; i < 4; ++i)
		{
			glm::ivec2 npos = current->pos + Direction::get2Di(i);
			if (room->getBlock(npos.x, npos.y) || findNode(closedSet, npos) != nullptr)
				continue;
			float cost = current->g + 1.0f;

			auto node = findNode(openSet, npos);
			if (node == nullptr)
			{
				node = new Node(npos, current);
				node->g = cost;
				node->h = dist(npos, ed);
				openSet.push_back(node);
			}
			else if (node->g > cost)
			{
				node->parent = current;
				node->g = cost;
			}
		}
		for (int i = 0; i < 4; ++i)
		{
			glm::ivec2 npos = current->pos + Direction::get2Di(i + 4);
			if (room->getBlock(npos.x, current->pos.y) || room->getBlock(current->pos.x, npos.y) || room->getBlock(npos.x, npos.y) || findNode(closedSet, npos) != nullptr)
			{
				continue;
			}
			float cost = current->g + 1.41f;

			auto node = findNode(openSet, npos);
			if (node == nullptr)
			{
				node = new Node(npos, current);
				node->g = cost;
				node->h = dist(npos, ed);
				openSet.push_back(node);
			}
			else if (node->g > cost)
			{
				node->parent = current;
				node->g = cost;
			}
		}
	}

	if (current->pos == ed)
	{
		while (current != nullptr)
		{
			ret.push_back(current->pos);
			current = current->parent;
		}
	}

	for (auto e : openSet)
	{
		delete e;
	}
	for (auto e : closedSet)
	{
		delete e;
	}
}

Pathfinder::Node* Pathfinder::findNode(std::vector<Node*> const& lis, glm::ivec2 pos) const
{
	for (auto node : lis)
	{
		if (node->pos == pos)
			return node;
	}
	return nullptr;
}

float Pathfinder::dist(glm::vec2 const& p1, glm::vec2 const& p2)
{
	return glm::length(p1 - p2);
}

Pathfinder::Node::Node(glm::ivec2 const& pos, Node* node) : pos(pos), parent(node)
{
	g = h = 0;
}