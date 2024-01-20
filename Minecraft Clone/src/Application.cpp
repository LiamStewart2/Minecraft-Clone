#include "Application.h"



#define SEED 56483765842

void Application::setFunctions(GLFWcursorposfun cursorFunction)
{
	glfwSetCursorPosCallback(window, cursorFunction);
}

Application::Application()
{
	if (!glfwInit())
	{
		std::cout << "GLFW INIT FAILED" << std::endl;
	}
	else
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		window = glfwCreateWindow(windowWidth, windowHeight, "Minecraft Clone", NULL, NULL);
		if (!window)
		{
			std::cout << "GLFW WINDOW INIT FAILED" << std::endl;
			glfwTerminate();
		}
		else
		{
			glfwMakeContextCurrent(window);
			glfwSwapInterval(0);

			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			if (glewInit() != GLEW_OK)
				std::cout << "GLEW INIT ERROR" << std::endl;
			else
			{
				std::cout << glGetString(GL_VERSION) << std::endl;

				camera = Camera(glm::vec3(8.0f, CHUNK_HEIGHT / 2 + 10, 8.0f));
				updatePlayerChunkPosition();

				terrainHeight = PerlinNoise(SEED);
				treeMap = PerlinNoise(SEED);
				mountainMap = PerlinNoise(SEED);

				initTriangle();
				initMap();
			}
		}
	}
}

void Application::updatePlayerChunkPosition()
{
	playerChunkPosition.x = (int)floor(camera.Position.x / CHUNK_WIDTH);
	playerChunkPosition.y = (int)floor(camera.Position.z / CHUNK_WIDTH);
}

Application::~Application()
{
}

void Application::mainloop()
{
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		frameCount++;

		if (currentFrame - previousTime >= 1.0f)
		{
			//std::cout << playerChunkPosition.x << ":" << playerChunkPosition.y << "\n";
			//std::cout << frameCount << std::endl;
			frameCount = 0;
			previousTime = currentFrame;
		}

		glm::vec2 lastChunkPosition = playerChunkPosition;
		updatePlayerChunkPosition();
		if (lastChunkPosition != playerChunkPosition)
		{
			updateChunks(&lastChunkPosition);
		}

		processInput(window);

		draw();

		glfwPollEvents();
	}

	terminate();
}

void Application::updateChunks(const glm::vec2* lastChunkPosition)
{
	//Load new chunks
	if (lastChunkPosition->x < playerChunkPosition.x)
	{
		//moved increase x
		for (int i = -renderDistance; i <= renderDistance; i++)
		{
			int index = returnIndexOfChunkByPosition(glm::vec2(lastChunkPosition->x - renderDistance, playerChunkPosition.y + i));
			ChunkMap[index].cleanup();
			ChunkMap.erase(ChunkMap.begin() + index);
			ChunkMap.push_back(Chunk(playerChunkPosition.x + renderDistance, playerChunkPosition.y + i, &blockData, &terrainHeight, &mountainMap));
		}
	}
	else if (lastChunkPosition->x > playerChunkPosition.x)
	{
		//moved decrease x
		for (int i = -renderDistance; i <= renderDistance; i++)
		{
			int index = returnIndexOfChunkByPosition(glm::vec2(lastChunkPosition->x + renderDistance, playerChunkPosition.y + i));
			ChunkMap[index].cleanup();
			ChunkMap.erase(ChunkMap.begin() + index);
			ChunkMap.push_back(Chunk(playerChunkPosition.x - renderDistance, playerChunkPosition.y + i, &blockData, &terrainHeight, &mountainMap));
		}
	}

	if (lastChunkPosition->y < playerChunkPosition.y)
	{
		//move increased y
		for (int i = -renderDistance; i <= renderDistance; i++)
		{
			int index = returnIndexOfChunkByPosition(glm::vec2(playerChunkPosition.x + i, lastChunkPosition->y - renderDistance));
			ChunkMap[index].cleanup();
			ChunkMap.erase(ChunkMap.begin() + index);
			ChunkMap.push_back(Chunk(playerChunkPosition.x + i, playerChunkPosition.y + renderDistance, &blockData, &terrainHeight, &mountainMap));
		}
	}
	else if(lastChunkPosition->y > playerChunkPosition.y)
	{
		//moved decreased y
		for (int i = -renderDistance; i <= renderDistance; i++)
		{
			int index = returnIndexOfChunkByPosition(glm::vec2(playerChunkPosition.x + i, lastChunkPosition->y + renderDistance));
			ChunkMap[index].cleanup();
			ChunkMap.erase(ChunkMap.begin() + index);
			ChunkMap.push_back(Chunk(playerChunkPosition.x + i, playerChunkPosition.y - renderDistance, &blockData, &terrainHeight, &mountainMap));
		}
	}
	updateAllChunkEdgeCases();
}

void Application::initTriangle()
{
	shader = Shader("src/Shaders/DefaultShader/shader.vs","src/Shaders/DefaultShader/shader.fs");
	/* OPENGL */

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
	
	//This Fuckery is for later
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CW);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	blockData = BlockDatabase();
	blockData.initDatabase();
}

void Application::updateAllChunkEdgeCases()
{
	for (int x = -renderDistance; x <= renderDistance; x++)
	{
		for (int y = -renderDistance; y <= renderDistance; y++)
		{
			int xPosition = playerChunkPosition.x + x;
			int yPosition = playerChunkPosition.y + y;

			int index = returnIndexOfChunkByPosition(glm::vec2(xPosition, yPosition));

			if (x != -renderDistance)
			{
				int negativeXindex = returnIndexOfChunkByPosition(glm::vec2(xPosition - 1, yPosition));
				ChunkMap[index].updateEdgeCases(&ChunkMap[negativeXindex]);
			}
			if (x != renderDistance)
			{
				int positiveXindex = returnIndexOfChunkByPosition(glm::vec2(xPosition + 1, yPosition));
				ChunkMap[index].updateEdgeCases(&ChunkMap[positiveXindex]);
			}

			if (y != -renderDistance)
			{
				int negativeYindex = returnIndexOfChunkByPosition(glm::vec2(xPosition, yPosition - 1));
				ChunkMap[index].updateEdgeCases(&ChunkMap[negativeYindex]);
			}
			if (y != renderDistance)
			{
				int positiveYindex = returnIndexOfChunkByPosition(glm::vec2(xPosition, yPosition + 1));
				ChunkMap[index].updateEdgeCases(&ChunkMap[positiveYindex]);
			}
		}
	}
}

void Application::initMap()
{
	for (int x = -renderDistance; x <= renderDistance; x++)
	{
		for (int y = -renderDistance; y <= renderDistance; y++)
		{
			ChunkMap.push_back(Chunk(x, y, &blockData, &terrainHeight, &mountainMap));
		}
	}
	if (true)
	{
		updateAllChunkEdgeCases();
	}
}

int Application::returnIndexOfChunkByPosition(glm::vec2 chunkPosition)
{
	for (int i = 0; i < ChunkMap.size(); i++)
	{
		if (chunkPosition == ChunkMap[i].chunk_pos)
			return i;
	}
	return -1;
}

void Application::draw()
{
	glClearColor(0.2f, 0.94f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.useShader();
	glBindVertexArray(VAO);

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(camera.Zoom), (float)(windowWidth / windowHeight), 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();

	shader.setMat4("view", view);
	shader.setMat4("projection", projection);

	

	drawMap();

	glfwSwapBuffers(window);
}

void Application::drawMap()
{
	for (int i = 0; i < ChunkMap.size(); i++)
	{
		ChunkMap[i].drawChunk(&shader);
	}
}

void Application::terminate()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	shader.terminateShader();

	glfwTerminate();
}

void Application::processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void Application::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; 

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}