#pragma once

#include <vector>
#include <glm/glm.hpp>

class ObjLoader
{

	public:
	ObjLoader();
	//~ObjLoader();
	bool load(const char* filename, std::vector<glm::vec4>& vertices, std::vector<glm::vec3>& normals, std::vector<unsigned short>& elements);
	
};

