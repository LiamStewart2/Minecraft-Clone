#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

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
	void setMat4(const std::string& name, glm::mat4 value) const;
private:
	// Debugging for the shaders initialization
	void debugShaderiv(unsigned int shaderivID, const char* Location);
	void debugProgramiv(unsigned int shaderProgramID);
};

