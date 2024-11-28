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
		std::vector<GLushort> elements;

		loader.load("Assets/susane.obj", vertices, normals, elements);
		

		std::cout << "Vertices size: " << vertices.size() << std::endl;
		std::cout << "Normals size: " << normals.size() << std::endl;
		std::cout << "Elements size: " << elements.size() << std::endl;


		/*GLint attribute_v_coord;
		GLint vbo_mesh_vertices;
		glEnableVertexAttribArray(attribute_v_coord);
		// Describe our vertices array to OpenGL (it can't guess its format automatically)
		glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh_vertices);
		glVertexAttribPointer(
			attribute_v_coord,  // attribute
			4,                  // number of elements per vertex, here (x,y,z,w)
			GL_FLOAT,           // the type of each element
			GL_FALSE,           // take our values as-is
			0,                  // no extra data between each position
			0                   // offset of first element
		);

		GLint attribute_v_normal;
		GLint vbo_mesh_normals;
		glEnableVertexAttribArray(attribute_v_normal);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh_normals);
		glVertexAttribPointer(
			attribute_v_normal, // attribute
			3,                  // number of elements per vertex, here (x,y,z)
			GL_FLOAT,           // the type of each element
			GL_FALSE,           // take our values as-is
			0,                  // no extra data between each position
			0                   // offset of first element
		);

		GLint ibo_mesh_elements;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_mesh_elements);
		int size;  glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);*/






	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec4), vertices.data(), GL_STATIC_DRAW);

	GLuint ebo;

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(GLushort), elements.data(), GL_STATIC_DRAW);


	Shader shaderProgram("vertex_shader.glsl", "fragment_shader.glsl");

	stbi_set_flip_vertically_on_load(true);

	/*GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);*/

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	int width, height, nrChannels;
	/*unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);*/


	//GLuint texture2;
	//glGenTextures(1, &texture2);
	//glBindTexture(GL_TEXTURE_2D, texture2);

	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	/*data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);*/

	shaderProgram.setVertexAttribPointer("position", 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);


	GLuint normalVBO;
	glGenBuffers(1, &normalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
	shaderProgram.setVertexAttribPointer("normal", 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);


	shaderProgram.setVertexAttribPointer("texCoord", 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));


	glBindVertexArray(0);

	shaderProgram.use();
	shaderProgram.set3Float("triangleColor", 0.0f, 1.0f, 0.0f);

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
        }

		

        const Uint8* keyState = SDL_GetKeyboardState(nullptr);
        camera.processKeyboardInput(keyState, deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaderProgram.use();

        view = camera.getViewMatrix();
        projection = glm::perspective(glm::radians(camera.getFov()), screenWidth / screenHeight, 0.1f, 100.0f);

        shaderProgram.setMat4("view", view);
        shaderProgram.setMat4("projection", projection);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_SHORT, 0);
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
