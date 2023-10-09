#pragma once

#include <glm/gtc/type_ptr.hpp>

class Direction
{
public:
	static glm::vec3 vec[4];
	static glm::ivec2 ivec[8];

	static glm::vec3 const& get3D(int);
	static glm::ivec2 const& get2Di(int);
};
