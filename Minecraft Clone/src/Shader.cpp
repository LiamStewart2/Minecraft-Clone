#include "Shader.h"

Shader::Shader()
{
	shaderID = -1;
}

Shader::Shader(const char* filepath)
{
	/* VERTEX SHADER LOADING */
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	debugShaderiv(vertexShader, "VERTEX SHADER");

	/* FRAGMENT SHADER LOADING */
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	debugShaderiv(fragmentShader, "FRAGMENT SHADER");

	/* SHADER LINKING */
	shaderID = glCreateProgram();
	glAttachShader(shaderID, vertexShader);
	glAttachShader(shaderID, fragmentShader);
	glLinkProgram(shaderID);
	debugProgramiv(shaderID);

	/* CLEANUP */
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader()
{
}

void Shader::debugShaderiv(unsigned int shaderivID, const char* Location)
{
	int success;
	char infoLog[512];
	glGetShaderiv(shaderivID, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderivID, 512, NULL, infoLog);
		std::cout << "SHADER LOAD ERROR :: " << Location << "\n" << infoLog << std::endl;
	}
}

void Shader::debugProgramiv(unsigned int shaderProgramID)
{
	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
		std::cout << "SHADER LINK ERROR\n" << infoLog << std::endl;
	}
}

void Shader::useShader()
{
	glUseProgram(shaderID);
}
void Shader::terminateShader()
{
	glDeleteProgram(shaderID);
}