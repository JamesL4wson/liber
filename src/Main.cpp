#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <Eigen/Core>
#include <exprtk/exprtk.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Function.h"
#include "MarchingCubes.h"
#include "Curve.h"
#include "Surface.h"

using namespace std::chrono;
using namespace Eigen;

static void FramebufferSizeCallback(GLFWwindow *window, int width, int height);
static void CursorPositionCallback(GLFWwindow* window, double mouseX, double mouseY);
static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
VectorXf sinCosFunction(const VectorXf &inputVect);
GLFWwindow *InitWindow();

Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));

const unsigned int WIDTH = 1200;
const unsigned int HEIGHT = 800;

float previousMouseX;
float previousMouseY;

int main()
{
	GLFWwindow *window = InitWindow();
	Shader shader(
		Shader::ReadShaderFile("src/shaders/vertex.glsl"), 
		Shader::ReadShaderFile("src/shaders/fragment.glsl")
	);
	shader.Use();

	Surface surface;

	Function sinCosSurface("sin cos", "sin(x0) + cos(x1) - x2", 3, 1, 1);
	Vector3f outsideBasis[3] = {{1.0f,0.0f,0.0f}, {0.0f,1.0f,0.0f}, {0.0f,0.0f,1.0f}};
	
	std::vector<Vector3f> vertices;
	std::vector<uint32_t> triangles;
	
	MarchingCubes::basis = outsideBasis;
	Vector3f initalPosition(-5,-5,-5);
	float currentStep = 5.0f;
	
	int times[5];

	auto start = high_resolution_clock::now();

	// MarchingCubes::CheckCube(*sinCosFunction, currentStep, initalPosition);
	MarchingCubes::MarchCubes(sinCosSurface, initalPosition);

	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	std::cout << duration.count() << std::endl;

	vertices = MarchingCubes::vertices;
	triangles = MarchingCubes::triangles;

	std::vector<glm::vec3> convertedVerts(vertices.size());
	for (int i = 0; i < vertices.size(); i++) 
	{
		convertedVerts[i][0] = vertices[i][0];
		convertedVerts[i][1] = vertices[i][1];
		convertedVerts[i][2] = vertices[i][2];
	}
	
	surface.UpdateMeshData(convertedVerts, triangles);
	
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glfwPollEvents();

		shader.SetValue("view", camera.GetViewMatrix());
		shader.SetValue("projection", camera.GetProjectionMatrix((float)WIDTH, (float)HEIGHT));
		shader.SetValue("color", glm::vec3(191.0f/256.0f, 44.0f/256.0f, 44.0f/256.0f));

		// curve.SetLineWidth();
		surface.Draw();

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

static void FramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

static void CursorPositionCallback(GLFWwindow *window, double mouseX, double mouseY)
{
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
	{	
		float mouseXDelta = mouseX - previousMouseX;
		float mouseYDelta = - mouseY + previousMouseY;
		
		bool rotate = false;
		state = glfwGetKey(window, GLFW_KEY_LEFT_SUPER);
		if (state == GLFW_PRESS)
		{
			rotate = true;
		}

		camera.CalculateMovement(mouseXDelta, mouseYDelta, rotate);
	}

	previousMouseY = mouseY;
	previousMouseX = mouseX;	
}

static void ScrollCallback(GLFWwindow *window, double xoffset, double yoffset) 
{
	camera.Zoom((float)yoffset);
}

GLFWwindow *InitWindow() 
{
	glfwInit();
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "liber", NULL, NULL);
	glfwMakeContextCurrent(window);
	
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	glfwSetCursorPosCallback(window, CursorPositionCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	glClearColor((float)30/255,(float)30/255,(float)30/255,1);

	return window;
}

VectorXf sinCosFunction(const VectorXf &inputVect) 
{
	VectorXf output(1);
	output[0] = std::sin(inputVect[0]) + std::cos(inputVect[1]) - inputVect[2];
	return output;
}
