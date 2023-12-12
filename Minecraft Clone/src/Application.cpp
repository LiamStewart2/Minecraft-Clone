#include "Application.h"

Application::Application()
{
	if (!glfwInit())
	{
		std::cout << "GLFW INIT FAILED" << std::endl;
	}
	else 
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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
			if (glewInit() != GLEW_OK)
				std::cout << "GLEW INIT ERROR" << std::endl;
			else
			{
				std::cout << glGetString(GL_VERSION) << std::endl;

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
		draw();

		glfwPollEvents();
	}

	terminate();
}

void Application::initTriangle()
{
	//store the shader source code
	//the ID of the shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//load the shader source code into the vertex shader ID
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//compile the shader
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "shader load error :: vertex shader\n" << infoLog << std::endl;
	}


	//Fragment Shader
	//the ID of the shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//load the shader source code into the vertex shader ID
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//compile the shader
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "shader load error :: fragment shader\n" << infoLog << std::endl;
	}


	// Shader Program;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "shader link error\n" << infoLog << std::endl;
	}
	/*
	glUseProgram(shaderProgram);
	*/

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/* NEXT BIT */


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Application::draw()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glfwSwapBuffers(window);
}

void Application::terminate()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
}
