#include <glad/glad.h>
#include <iostream>

#include "Shader.h"

using namespace Eigen;

Shader::Shader() {}

void Shader::Compile(const char** shaderSrc, int srcCount)
{
	programId = glCreateProgram();

	size_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &shaderSrc[0], nullptr);
	glCompileShader(vertexShader);
	glAttachShader(programId, vertexShader);
	
	size_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &shaderSrc[1], nullptr);
	glCompileShader(fragmentShader);
	glAttachShader(programId, fragmentShader);

	char infoLog[1024]{};
	int success{};

	if (srcCount == 3)
	{
		size_t geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShader, 1, &shaderSrc[2], nullptr);
		glCompileShader(geometryShader);
		glAttachShader(programId, geometryShader);

		glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
		if (!success) 
		{
			glGetShaderInfoLog(geometryShader, 1024, nullptr, infoLog); 
			std::cerr << "failed to compile geometry shader! \nInfoLog:\n" << infoLog;
		}
	}
	
	glLinkProgram(programId); 

	//REPORT COMPILER/LINKER ERRORS
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) 
	{
		glGetShaderInfoLog(vertexShader, 1024, nullptr, infoLog); 
		std::cerr << "failed to compile vertex shader! \nInfoLog:\n" << infoLog;
	}

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) 
	{
		glGetShaderInfoLog(fragmentShader, 1024, nullptr, infoLog); 
		std::cerr << "failed to compile fragment shader! \nInfoLog:\n" << infoLog;
	}

	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success) 
	{
		glGetProgramInfoLog(programId, 1024, nullptr, infoLog); 
		std::cerr << "failed to link shader! \nInfoLog:\n" << infoLog;
	}

	//CLEAN UP SHADER SOURCE
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::SetValue(const std::string& name, float value) 
{
	glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::SetValue(const std::string& name, Vector2f value) 
{
	glUniform2f(glGetUniformLocation(programId, name.c_str()), value[0], value[1]);
}

void Shader::SetValue(const std::string& name, Vector3f values) 
{
	glUniform3f(glGetUniformLocation(programId, name.c_str()), values[0], values[1], values[2]);
}

void Shader::SetValue(const std::string& name, Matrix4f matrix) 
{
	glUniformMatrix4fv(glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, matrix.data());
}
 