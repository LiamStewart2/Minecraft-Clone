#include "Application.h"


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

				camera = Camera(glm::vec3(8.0f, 18.0f, 8.0f));

				initTriangle();
				initMap();
			}
		}
	}
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
			std::cout << camera.Position.x / CHUNK_WIDTH << ":" << camera.Position.z / CHUNK_DEPTH << "\n";
			//std::cout << frameCount << std::endl;
			frameCount = 0;
			previousTime = currentFrame;
		}

		processInput(window);

		draw();

		glfwPollEvents();
	}

	terminate();
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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	blockData = BlockDatabase();
	blockData.initDatabase();
}

void Application::initMap()
{
	for (int x = 0; x < worldSize; x++)
	{
		for (int y = 0; y < worldSize; y++)
		{
			ChunkMap.emplace_back(Chunk(x, y, &blockData));
			std::cout << x << ":" << y << "; \n";
			if (x > 0)
			{
				ChunkMap[0].updateEdgeCases(&ChunkMap[1], LEFT_SIDE);
			}
		}
	}

	for (int x = 0; x < worldSize; x++)
	{
		for (int y = 0; y < worldSize; y++)
		{
			int index = x + (y * worldSize);
			if (x > 0) {
				ChunkMap[index].updateEdgeCases(&ChunkMap[x - 1 + (y * worldSize)], RIGHT_SIDE);
			}
			if (y > 0){
				ChunkMap[index].updateEdgeCases(&ChunkMap[x + ((y - 1) * worldSize)], FRONT_SIDE);
			}
			if (x < worldSize - 1) {
				ChunkMap[index].updateEdgeCases(&ChunkMap[x + 1 + (y * worldSize)], LEFT_SIDE);
			}
			if (y > worldSize - 1) {
				ChunkMap[index].updateEdgeCases(&ChunkMap[x + ((y + 1) * worldSize)], BACK_SIDE);
			}
		}
	}
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