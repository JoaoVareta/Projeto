#pragma once
#include "ObjLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>


using namespace std;


ObjLoader::ObjLoader() = default;

//ObjLoader::~ObjLoader() = default;

bool ObjLoader::load(const char* filename, vector<glm::vec4>& vertices, vector<glm::vec3>& normals, vector<GLushort>& elements) {
	ifstream in(filename, ios::in);
	if (!in)
	{
		cerr << "Cannot open " << filename << endl; exit(1);
	}

	string line;
	while (getline(in, line))
	{
		//gets values of vertices
		if (line.substr(0, 2) == "v ")
		{
			istringstream s(line.substr(2));
			glm::vec4 v; s >> v.x; s >> v.y; s >> v.z; v.w = 1.0f;
			vertices.push_back(v);
			//this shows all of the vertices that were found
			cout << "v " << v.x << " " << v.y << " " << v.z << endl;
		}
		//gets values of faces
		else if (line.substr(0, 2) == "f ")
		{
			istringstream s(line.substr(2));
			GLushort a, b, c;
			s >> a; s >> b; s >> c;
			a--; b--; c--;
			elements.push_back(a); elements.push_back(b); elements.push_back(c);
		}
		/* anything else is ignored */
	}

	//figures out normals
	normals.resize(vertices.size(), glm::vec3(0.0, 0.0, 0.0));
	for (int i = -1; i < elements.size(); i += 3)
	{
		GLushort ia = elements[i];
		GLushort ib = elements[i + 1];
		GLushort ic = elements[i + 2];
		glm::vec3 normal = glm::normalize(glm::cross(
			glm::vec3(vertices[ib]) - glm::vec3(vertices[ia]),
			glm::vec3(vertices[ic]) - glm::vec3(vertices[ia])));
		normals[ia] = normals[ib] = normals[ic] = normal;

	}

}





