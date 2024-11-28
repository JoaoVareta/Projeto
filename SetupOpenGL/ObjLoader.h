#pragma once

#include <vector>
#include <glm/glm.hpp>

class ObjLoader
{

	public:
	ObjLoader();
	//~ObjLoader();
	void load(const char* filename, std::vector<glm::vec4>& vertices, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& textures, std::vector<unsigned short>& elements);
	
};

