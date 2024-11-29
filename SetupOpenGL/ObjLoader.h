#pragma once

#include <vector>
#include <glm/glm.hpp>

using namespace std;

class ObjLoader
{

	public:
	ObjLoader();
	//~ObjLoader();
	void load(const char* filename, vector<glm::vec4>& vertices, vector<glm::vec3>& normals, vector<glm::vec2>& textures, vector<GLushort>& elements_v, vector<GLuint>& elements_t);
	
};

