#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Shader.h"
#include "Block.h"
#include "Camera.h"

#include "BlockType.h"
#include "Chunk.h"
#include "BlockDatabase.h"
#include "PerlinNoise.h"

class Application
{
public:
	Application();
	~Application();

	void mainloop();

    void processInput(GLFWwindow* window);
    void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

    void setFunctions(GLFWcursorposfun cursorFunction);
private:
	GLFWwindow* window;

	int windowWidth = 1000; int windowHeight = 1000;

    //Cube vertices
    float vertices[180] = {
        //Back Face
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        //Front Face
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        //Left Face
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 1.0f,

        //Right Face
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,

        //Bottom Face
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        //Top Face
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    std::vector<Chunk> ChunkMap;

    int worldSize = 3;
    int renderDistance = 4;

    Camera camera;
	Shader shader;

    glm::vec2 playerChunkPosition;

    PerlinNoise Perlin;

    BlockDatabase blockData;

	unsigned int VBO; // vertx buffer object
	unsigned int VAO; // vertex array object

    bool firstMouse = true;
    float lastX = ((float)windowWidth) / 2;
    float lastY = ((float)windowHeight) / 2;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    int frameCount = 0;
    double previousTime = glfwGetTime();

	void initTriangle();
    void initMap();
	void draw();
    void drawMap();
	void terminate();

    void updateAllChunkEdgeCases();
    void updateChunks(const glm::vec2* lastChunkPosition);
    void updatePlayerChunkPosition();
    int returnIndexOfChunkByPosition(glm::vec2 position);
};

