#include "Application.h"

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
	/*
	glGenBuffers(1, &VBO);
	//binds the buffer we are working with
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//bind the vertices to the ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	*/

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

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

void Application::draw()
{
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glfwSwapBuffers(window);
}

void Application::terminate()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
}
