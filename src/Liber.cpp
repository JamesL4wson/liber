#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Curve.h"
#include "Shader.h"
#include "Plot.h"
#include "GUI.h"

void InitWindow();
void UpdateLoop();
void DrawModels();
void FramebufferSizeCallback(GLFWwindow *window, int width, int height);
void CursorPositionCallback(GLFWwindow *window, double mouseX, double mouseY);
void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset);

float WIDTH = 1200;
float HEIGHT = 800;

float previousMouseX;
float previousMouseY;

Camera camera;
Shader surfaceShader;
Shader curveShader;
Plot plot;
GLFWwindow *window;

int main() 
{
	InitWindow();
	UpdateLoop();
}

void UpdateLoop() 
{
	std::vector<Variable> variables;
	Variable var = {-5, 5, *"x", 0};
	variables.push_back(var);
	var = {-5, 5, *"y", 0};
	variables.push_back(var);
	var = {-5, 5, *"z", 0};
	variables.push_back(var);
	// var = {-5, 5, *"w", 0};
	// variables.push_back(var);

	plot.globalVariables = variables;
	Function::globalVariables = plot.globalVariables;
	
    while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		glfwPollEvents();
		
		DrawModels();
		

		GUI::DrawUI(plot);
		
		glfwSwapBuffers(window);
	}

	GUI::ShutdownUI();
    glfwTerminate();
}

//initalization
void InitWindow() 
{
	//GLFW / OPENGL
	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	window = glfwCreateWindow(WIDTH, HEIGHT, "liber", NULL, NULL);
	glfwMakeContextCurrent(window);
	
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	glfwSetCursorPosCallback(window, CursorPositionCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	glEnable(GL_DEPTH_TEST);
	glClearColor(34.0f/255, 34.0f/255, 34.0f/255, 1.0f);
	
	//IMGUI
	GUI::InitalizeUI(window);

	//SHADERS
	surfaceShader.Compile(ShaderSrc::surfaceShaderSrc, 2);
	curveShader.Compile(ShaderSrc::curveShaderSrc, 3);
}

//models
void DrawModels()
{
	
	for (size_t i = 0; i < plot.functions.size(); i++)
	{
		glUseProgram(surfaceShader.programId);
		surfaceShader.SetValue("lightPos", camera.position);
		surfaceShader.SetValue("lightColor", Eigen::Vector3f(1.0f, 1.0f, 1.0f));
		surfaceShader.SetValue("ambientStrength", 0.5f);
		surfaceShader.SetValue("view", camera.GetViewMatrix());
		surfaceShader.SetValue("projection", camera.GetProjectionMatrix((float)WIDTH, (float)HEIGHT));
		surfaceShader.SetValue("cameraPos", camera.position);
		
		plot.functions[i].Draw();
	
		glUseProgram(curveShader.programId);
		curveShader.SetValue("viewportSize", Eigen::Vector2f(WIDTH, HEIGHT));
		curveShader.SetValue("width", 4);
		curveShader.SetValue("view", camera.GetViewMatrix());
		curveShader.SetValue("projection", camera.GetProjectionMatrix((float)WIDTH, (float)HEIGHT));
		
		plot.functions[0].Draw2();
	}
}

//input detection
void FramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);

	WIDTH = width;
	HEIGHT = height;
}

void CursorPositionCallback(GLFWwindow *window, double mouseX, double mouseY)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{	
		float mouseXDelta = mouseX - previousMouseX;
		float mouseYDelta = mouseY - previousMouseY;
		
		if (glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS)
		{
			camera.Rotate(mouseXDelta, mouseYDelta);
		}
		else
		{
			camera.Translate(mouseXDelta, mouseYDelta);
		}
	}

	previousMouseY = mouseY;
	previousMouseX = mouseX;	
}

void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset) 
{
	camera.Zoom((float)yoffset);
}
