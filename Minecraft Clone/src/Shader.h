#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>;

class Shader
{
public:
	//Default Shader Constructor
	Shader();
	//Filepath takes the location of the file
	Shader(const char* filepath);
	~Shader();

	unsigned int shaderID;

	void useShader();
	//To be called at the end of program
	void terminateShader();
private:
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	// Debugging for the shaders initialization
	void debugShaderiv(unsigned int shaderivID, const char* Location);
	void debugProgramiv(unsigned int shaderProgramID);
};

