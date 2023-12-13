#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	//Default Shader Constructor
	Shader();
	//Filepath takes the location of the file
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	unsigned int shaderID;

	void useShader();
	//To be called at the end of program
	void terminateShader();

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
private:
	/*
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec3 oColor;"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(oColor, 1.0f);\n"
		"}\0";
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"
		"out vec3 oColor;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"   oColor = aColor;\n"
		"}\0";
	*/

	// Debugging for the shaders initialization
	void debugShaderiv(unsigned int shaderivID, const char* Location);
	void debugProgramiv(unsigned int shaderProgramID);
};

