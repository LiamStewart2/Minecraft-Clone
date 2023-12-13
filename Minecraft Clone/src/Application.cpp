#include "Application.h"

void Application::setFunctions(GLFWcursorposfun cursorFunction, GLFWscrollfun scrollFunction)
{
	glfwSetCursorPosCallback(window, cursorFunction);
	glfwSetScrollCallback(window, scrollFunction);
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

	grassTexture = Texture("res/images/blocks/grass.jpg");
	cobblestoneTexture = Texture("res/images/blocks/cobblestone.jpg");
	

	/* OPENGL */

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	
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

void Application::draw()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	shader.useShader();
	glBindVertexArray(VAO);

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), (float)(windowWidth / windowHeight), 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();

	cobblestoneTexture.useTexture();
	shader.setMat4("view", view);
	shader.setMat4("projection", projection);
	shader.setInt("ourTexture", 0);

	for(int y = -2; y < 0; y++)
	{
		if (y == -1)
			grassTexture.useTexture();
		else
			cobblestoneTexture.useTexture();
		for (int i = -worldSize; i < worldSize + 1; i++)
		{
			for (int j = -worldSize; j < worldSize + 1; j++)
			{
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(i, y, j));
				shader.setMat4("model", model);

				glDrawArrays(GL_TRIANGLES, 0, 36);
				GLenum error = glGetError();
				if (error != GL_NO_ERROR)
				{
					std::cout << "OpenGL Error: " << error << std::endl;
				}
			}
		}
	}

	glfwSwapBuffers(window);
}

void Application::terminate()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
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

void Application::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}