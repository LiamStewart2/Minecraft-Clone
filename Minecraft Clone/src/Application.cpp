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

				camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

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
			std::cout << frameCount << std::endl;
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

	tGrassSide = Texture("res/images/blocks/grass_block_side.png");
	tGrassTop = Texture("res/images/blocks/grass_block_top.png");
	tCobblestone = Texture("res/images/blocks/cobblestone.png");
	
	bGrass = BlockType(&tGrassSide, &tGrassTop, color(69, 255, 81), color(), color(156, 63, 37));
	bCobblestone = BlockType(&tCobblestone);
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
}

void Application::initMap()
{

	color white = color();
	Map = std::vector<Block>();
	for (int y = -worldDepth; y < 0; y++)
	{
		for (int x = -worldSize / 2; x++; x <= worldSize)
		{
			for (int z = -worldSize / 2; z++; z <= worldSize)
			{
				if (y == -1)
				{
					Map.push_back(Block(&bGrass, glm::vec3(x, y, z)));
				}
				else
					Map.push_back(Block(&bCobblestone, glm::vec3(x, y, z)));

				Block* lastBlock = &(Map.back());
				lastBlock->updateFace(TOP_SIDE,     y == -1);
				lastBlock->updateFace(BOTTOM_SIDE,  y == -worldDepth);
				lastBlock->updateFace(LEFT_SIDE,    x == (-worldSize / 2) + 1);
				lastBlock->updateFace(BACK_SIDE,    z == (-worldSize / 2) + 1);
				lastBlock->updateFace(RIGHT_SIDE,   x ==  0);
				lastBlock->updateFace(FRONT_SIDE,   z ==  0);
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
	for (int i = 0; i < Map.size(); i++)
	{
		Map[i].draw(&shader);
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