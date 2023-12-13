#include "Shader.h"

Shader::Shader()
{
	shaderID = -1;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	/* LOAD SHADERS FROM FILE */
	std::string vertexCode, fragmentCode;
	std::ifstream vShaderFile, fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SAHDER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}

	const char* vertexShaderSource = vertexCode.c_str();
	const char* fragmentShaderSource = fragmentCode.c_str();

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


void Shader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(shaderID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(shaderID, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(shaderID, name.c_str()), value);
}
void Shader::setMat4(const std::string& name, glm::mat4 value) const {
	glUniformMatrix4fv(glGetUniformLocation(shaderID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
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