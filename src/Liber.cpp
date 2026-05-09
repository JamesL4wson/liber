#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
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
Shader shader;
Plot plot;
GLFWwindow *window;

int main() 
{
	InitWindow();
	UpdateLoop();
}

void UpdateLoop() 
{
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

	//SHADER
	shader.Compile();
}

//models
void DrawModels()
{
	shader.SetValue("lightPos", Vector3f(0, 0.0, 3.0f));
	shader.SetValue("lightColor", Vector3f(1.0f, 1.0f, 1.0f));
	shader.SetValue("ambientStrength", 0.5f);
	shader.SetValue("view", camera.GetViewMatrix());
	shader.SetValue("projection", camera.GetProjectionMatrix((float)WIDTH, (float)HEIGHT));
	shader.SetValue("cameraPos", camera.position);
	
	for (size_t i = 0; i < plot.functions.size(); i++)
	{
		plot.functions[i].Draw();
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
