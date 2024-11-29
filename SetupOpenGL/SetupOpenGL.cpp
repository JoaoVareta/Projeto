#include <SDL.h>
#include <glad/glad.h>
#include <iostream>
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>


#include "Shader.h"
#include "Camera.h"
#include "ObjLoader.h"

using namespace std;

int main(int argc, char** argv)
{

	float screenWidth = 800;
	float screenHeight = 600;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_Window* window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	SDL_CaptureMouse(SDL_TRUE);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		SDL_Quit();
		return -2;
	}

	ObjLoader loader;

		std::vector<glm::vec4> vertices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> textures;
		vector<GLushort> elements_v;
		vector<GLuint> elements_t;

		loader.load("Assets/susane.obj", vertices, normals, textures, elements_v, elements_t);

	
		std::cout << "Vertices size: " << vertices.size() << std::endl;
		std::cout << "Normals size: " << normals.size() << std::endl;
		std::cout << "Elements size: " << elements_v.size() << std::endl;

		vector<float> ObjLoad;

		for (size_t i = 0; i < elements_v.size(); i++)
		{
			ObjLoad.push_back(vertices[elements_v[i]].x);
			ObjLoad.push_back(vertices[elements_v[i]].y);
			ObjLoad.push_back(vertices[elements_v[i]].z);
			ObjLoad.push_back(textures[elements_t[i]].x);
			ObjLoad.push_back(textures[elements_t[i]].y);
		}

		
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, ObjLoad.size() * sizeof(glm::vec4), ObjLoad.data(), GL_STATIC_DRAW);

	GLuint ebo;

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements_v.size() * sizeof(GLushort), elements_v.data(), GL_STATIC_DRAW);

	Shader shaderProgram("vertex_shader.glsl", "fragment_shader.glsl");

	stbi_set_flip_vertically_on_load(true);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	shaderProgram.setVertexAttribPointer("position", 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	shaderProgram.setVertexAttribPointer("texCoord", 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(0);

	shaderProgram.use();
	shaderProgram.setInt("susanne", 0);

	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

	glm::mat4 view = camera.getViewMatrix();
	glm::mat4 model(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 projection = glm::perspective(glm::radians(camera.getFov()), screenWidth / screenHeight, 0.1f, 100.0f);

	shaderProgram.setMat4("model", model);
	shaderProgram.setMat4("view", view);
	shaderProgram.setMat4("projection", projection);

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);



	SDL_Event event;

	bool isRunning = true;
	int start = SDL_GetTicks();
	float deltaTime = 0.0f;
	float lastFrameTime = start; // Time of last frame



	glEnable(GL_DEPTH_TEST);


	while (isRunning) // Render Loop
    {
        int now = SDL_GetTicks();
        deltaTime = (now - lastFrameTime) / 1000.0f;
        lastFrameTime = now;

        while (SDL_PollEvent(&event) != 0) // Event Loop
        {
            if (event.type == SDL_QUIT) isRunning = false;

            if (event.type == SDL_MOUSEMOTION) {
                camera.processMouseInput(event.motion.x, event.motion.y);
            }

            if (event.type == SDL_MOUSEWHEEL) {
                camera.processMouseScroll(event.wheel.y);
            }

			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
					isRunning = false;
					break;
					// Handle other keys
				}
			}
        }

		

        const Uint8* keyState = SDL_GetKeyboardState(nullptr);
        camera.processKeyboardInput(keyState, deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaderProgram.use();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

        view = camera.getViewMatrix();
        projection = glm::perspective(glm::radians(camera.getFov()), screenWidth / screenHeight, 0.1f, 100.0f);

        shaderProgram.setMat4("view", view);
        shaderProgram.setMat4("projection", projection);



        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, elements_v.size(), GL_UNSIGNED_SHORT, 0);
        glBindVertexArray(0);
		//glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        SDL_GL_SwapWindow(window);
    }

    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    glDeleteVertexArrays(1, &vao);
	glDisable(GL_CULL_FACE);


    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
