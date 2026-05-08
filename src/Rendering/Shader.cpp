#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Shader.h"

const char* fragmentShaderCode = {
	"#version 330 core\n"

	"in vec3 Position;"
	"in vec3 Normal;"
	"in vec3 Color;"

	"out vec4 FragColor;"

	"uniform float ambientStrength;"

	"uniform vec3 lightColor;"
	"uniform vec3 lightPos;"
	"uniform vec3 cameraPos;"

	"void main()"
	"{"
		"float Ia = ambientStrength; "

		"vec3 N = normalize(Normal); "
		"vec3 Lm = normalize(lightPos - Position);"
		"float Id = max(dot(N, Lm), 0.0);"

		"vec3 V = normalize(cameraPos - Position);"
		"vec3 Rm = reflect(-Lm, N);"
		"float Is = pow(max(dot(Rm, V), 0.0), 16) * 0.5;"

		"vec3 finalColor = (Ia + Id + Is) * lightColor * Color; "

		"FragColor = vec4(finalColor, 1.0);"
	"}"};

const char* vertexShaderCode = {
	"#version 330 core\n"

	"layout (location = 0) in vec3 pos;"
	"layout (location = 1) in vec3 norm;"
	"layout (location = 2) in vec3 col;"

	"uniform mat4 projection;"
	"uniform mat4 view;"

	"out vec3 Normal;"
	"out vec3 Position;"
	"out vec3 Color;"

	"void main() "
	"{"
		"gl_Position = projection * view * vec4(pos, 1.0);"

		"Normal = norm;"
		"Position = pos;"
		"Color = col;"
	"}"
};

Shader::Shader() {}

void Shader::Compile()
{
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

	glUseProgram(programId);
}

void Shader::SetValue(const std::string& name, float value) 
{
	glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
}


void Shader::SetValue(const std::string& name, glm::vec3 values) 
{
	glUniform3f(glGetUniformLocation(programId, name.c_str()), values.x, values.y, values.z);
}

void Shader::SetValue(const std::string& name, glm::mat4 matrix) 
{
	glUniformMatrix4fv(glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}
 