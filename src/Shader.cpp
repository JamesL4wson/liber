#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream> 
#include <iostream>
#include "Shader.h"

Shader::Shader(const std::string& vertexCode, const std::string& fragmentCode) 
{
    const char* vertexShaderCode = vertexCode.c_str();
    const char* fragmentShaderCode = fragmentCode.c_str();

    char infoLog[1024]{};
    int success{};

    size_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) 
	{
		glGetShaderInfoLog(vertexShader, 1024, nullptr, infoLog); 
		std::cerr << "failed to compile shader! \nInfoLog:\n" << infoLog;
	}

	size_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderCode, nullptr);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) 
	{
		glGetShaderInfoLog(fragmentShader, 1024, nullptr, infoLog); 
		std::cerr << "failed to compile shader! \nInfoLog:\n" << infoLog;
	}

	programId = glCreateProgram();
	glAttachShader(programId, vertexShader);
	glAttachShader(programId, fragmentShader);
	glLinkProgram(programId); 

	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success) 
	{
		glGetProgramInfoLog(programId, 1024, nullptr, infoLog); 
		std::cerr << "failed to link shader! \nInfoLog:\n" << infoLog;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

std::string Shader::ReadShaderFile(const std::string& fileName) 
{
	std::ifstream file(fileName);
	std::stringstream shaderString{};

	shaderString << file.rdbuf();

	return shaderString.str();
}

void Shader::Use() 
{
	glUseProgram(programId);
}

void Shader::SetValue(const std::string& name, glm::vec3 values) 
{
	glUniform3f(glGetUniformLocation(programId, name.c_str()), values.x, values.y, values.z);
}

void Shader::SetValue(const std::string& name, glm::mat4 matrix) 
{
	glUniformMatrix4fv(glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}
 