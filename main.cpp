#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <openglErrorReporting.h>
#include "renderer.h"
#include "fpsCameraController.h"

// Global controller to tie the callbacks (Perhaps change later to have no globals)
FpsCameraController* g_controller = nullptr;
bool windowClose = false;
#include <iostream>

// Input callbacks
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	static bool firstMouse = true;
	static float lastX = xpos;
	static float lastY = ypos;

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
		return;
	}

	float dx = xpos - lastX;
	float dy = ypos - lastY;

	lastX = xpos;
	lastY = ypos;

	if (g_controller) g_controller->OnMouseMove(dx, dy);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE) windowClose = true;
	if (g_controller) g_controller->OnKeyPress(key, action);
}

int main()
{
	if (!glfwInit())
	{
		std::cout << "GLFW Init Error!\n";
		return 1;
	}

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#endif

	GLFWwindow *window = glfwCreateWindow(640, 480, "Falcon-PBR", NULL, NULL);
	if (!window)
	{
		std::cout << "Window Error!\n";
		return 1;
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return 1;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	enableReportGlErrors();

	// Enable culling and depth
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	// Camera related
	Camera camera(45.0f, 640.0f / 480.0f, 0.01f, 100.0f);
	camera.SetPosition({ 0.0f, 0.0f, 2.0f });
	FpsCameraController controller(&camera);
	g_controller = &controller;

	// Load Damaged Helmet
	GLTFScene damagedHelmet(std::string(RESOURCES_PATH) + "models/gltf/DamagedHelmet.glb");
	// Create shaders and shader program for the damagedHelmet
	Shader damagedHelmetProgram((std::string(RESOURCES_PATH) + "shaders/phongVertex.vert").c_str(),
		(std::string(RESOURCES_PATH) + "shaders/phongFragment.frag").c_str());

	LightValues dir{};
	dir.type = LightcasterType::DIRECTIONAL_LIGHT;
	dir.colorIntensity = 0.2f;
	dir.color = glm::vec4(1.0f);
	dir.lightDir = glm::vec4(glm::normalize(glm::vec3(-1.0f, 10.0f, -1.0f)), 0.0f);
	dir.spotlight = glm::vec2(0.0f);
	dir.attenuation = glm::vec4(0.0f);

	LightValues point{};
	point.type = LightcasterType::POINT_LIGHT;
	point.colorIntensity = 1.0f;
	point.color = glm::vec4(1.0f, 0.2f, 0.2f, 1.0f);
	point.lightDir = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	point.spotlight = glm::vec2(0.0f);
	point.attenuation = glm::vec4(1.0f, 0.7f, 1.8f, 0.0f);
	
	LightValues spot{};
	spot.type = LightcasterType::FLASH_LIGHT;
	spot.colorIntensity = 0.8f;
	spot.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	spot.lightDir = glm::vec4(camera.GetPosition(), 1.0f);
	spot.spotlight = glm::vec2(glm::cos(glm::radians(50.0f)), glm::cos(glm::radians(60.0f)));
	spot.attenuation = glm::vec4(1.0f, 0.7f, 1.8f, 0.0f);

	// Set input callbacks
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);

	// Create Renderer
	Renderer renderer(&camera);
	renderer.InsertEntity(&damagedHelmetProgram, &damagedHelmet);
	renderer.AddLightObject(dir);
	renderer.AddLightObject(point);
	renderer.AddLightObject(spot);

	// Delta time 
	float lastFrameTime = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		if (windowClose) break;

		// Delta time logic
		float currentTime = static_cast<float>(glfwGetTime());
		float deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.render();
		
		controller.Update(deltaTime);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);

	glfwTerminate();

	return 0;
}

