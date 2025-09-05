#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <openglErrorReporting.h>
#include "renderer.h"
#include "fpsCameraController.h"

// Global controller to tie the callbacks (Perhaps change later to have no globals)
FpsCameraController* g_controller = nullptr;
bool windowClose = false;

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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
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
	camera.SetPosition({ 0.0f, 2.0f, 2.0f });
	FpsCameraController controller(&camera);
	g_controller = &controller;

	// Load Damaged Helmet
	std::unique_ptr<GLTFScene> damagedHelmet = std::make_unique<GLTFScene>(std::string(RESOURCES_PATH) + "models/gltf/DamagedHelmet.glb");

	// Create shaders and shader program for the damagedHelmet
	std::unique_ptr<Shader> damagedHelmetProgram = std::make_unique<Shader>(
		(std::string(RESOURCES_PATH) + "shaders/vertexGltf.vert").c_str(),
		(std::string(RESOURCES_PATH) + "shaders/fragmentGltf.frag").c_str()
	);

	// Model transform
	glm::mat4 model = glm::mat4(1.0f);
	std::unique_ptr<Transform> damagedHelmetTransform = std::make_unique<Transform>(model);
	damagedHelmetTransform->TranslateLocal(glm::vec3(0.0f, 3.0f, 0.0f));

	// Load Sponza Palace
	//GLTFScene sponza(std::string(RESOURCES_PATH) + "models/gltf/sponza.glb");
	//// Create shaders and shader program for the damagedHelmet
	//Shader sponzaProgram(
	//	(std::string(RESOURCES_PATH) + "shaders/vertexGltf.vert").c_str(),
	//	(std::string(RESOURCES_PATH) + "shaders/fragmentGltf.frag").c_str()
	//);

	//Transform sponzaTransform(model);
	//sponzaTransform.TranslateLocal(glm::vec3(0.0f, 3.0f, 0.0f));

	// Lamda as the helper for Transform population
	auto MakeTransform = [](const glm::vec3& pos, glm::vec3 forward) {
		forward = glm::normalize(forward);
		glm::vec3 up = (std::abs(glm::dot(forward, glm::vec3(0, 1, 0))) > 0.999f) ? glm::vec3(0, 0, 1) : glm::vec3(0, 1, 0);
		glm::vec3 right = glm::normalize(glm::cross(up, forward));
		glm::vec3 newUp = glm::cross(forward, right);
		glm::mat4 M(1.0f);
		M[0] = glm::vec4(right, 0.0f);
		M[1] = glm::vec4(newUp, 0.0f);
		M[2] = glm::vec4(forward, 0.0f);
		M[3] = glm::vec4(pos, 1.0f);
		return Transform(M);
	};

	// Directional light
	LightDesc dir{};
	dir.type = LightcasterType::DIRECTIONAL_LIGHT;
	dir.color = glm::vec3(1.0f);
	dir.intensity = 0.3f;
	dir.attenuation = glm::vec3(0.0f);
	dir.spotAnglesCos = glm::vec2(0.0f);
	dir.transform = MakeTransform(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// Point light
	LightDesc point{};
	point.type = LightcasterType::POINT_LIGHT;
	point.color = glm::vec3(1.0f, 0.2f, 0.2f);
	point.intensity = 1.0f;
	point.attenuation = glm::vec3(1.0f, 0.22f, 0.20f);
	point.spotAnglesCos = glm::vec2(0.0f);
	point.transform = MakeTransform(glm::vec3(0.5f, 1.5f, 0.5f), glm::vec3(0, 0, 1));
	
	// Spot light (at camera, pointing forward)
	LightDesc spot{};
	spot.type = LightcasterType::SPOT_LIGHT;
	spot.color = glm::vec3(1.0f);
	spot.intensity = 0.8f;
	spot.attenuation = glm::vec3(1.0f, 0.007f, 0.0002f);
	spot.spotAnglesCos = glm::vec2(glm::cos(glm::radians(50.0f)),
		glm::cos(glm::radians(60.0f)));
	spot.transform = MakeTransform(camera.GetPosition(), camera.GetForward());

	// Input callbacks
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);

	// Now pass everything to renderer
	Renderer renderer(&camera);
	renderer.InsertEntity(std::move(damagedHelmetProgram), std::move(damagedHelmet), std::move(damagedHelmetTransform), false);
	//renderer.InsertEntity(&sponzaProgram, &sponza, &sponzaTransform);

	renderer.CreateFloor();

	std::vector<std::string> faces
	{
		std::string(RESOURCES_PATH) + "textures/skybox/right.jpg",
		std::string(RESOURCES_PATH) + "textures/skybox/left.jpg",
		std::string(RESOURCES_PATH) + "textures/skybox/top.jpg",
		std::string(RESOURCES_PATH) + "textures/skybox/bottom.jpg",
		std::string(RESOURCES_PATH) + "textures/skybox/front.jpg",
		std::string(RESOURCES_PATH) + "textures/skybox/back.jpg"
	};

	renderer.CreateSkyBox(faces);

	int directionLightId = renderer.AddLightObject(dir);
	int pointLightId = renderer.AddLightObject(point);
	int spotLightId = renderer.AddLightObject(spot);

	// Delta time 
	float lastFrameTime = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		if (windowClose) break;

		// Delta time logic
		float currentTime = static_cast<float>(glfwGetTime());
		float deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;

		glClearColor(0.02f, 0.02f, 0.02f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Spot light (at camera, pointing forward)
		spot.type = LightcasterType::SPOT_LIGHT;
		spot.color = glm::vec3(1.0f);
		spot.intensity = 0.8f;
		spot.attenuation = glm::vec3(1.0f, 0.007f, 0.0002f);
		spot.spotAnglesCos = glm::vec2(glm::cos(glm::radians(12.0f)),
			glm::cos(glm::radians(15.0f)));
		spot.transform = MakeTransform(camera.GetPosition(), camera.GetForward());

		renderer.EditLightObject(spotLightId, spot);

		renderer.render();
		
		controller.Update(deltaTime);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);

	glfwTerminate();

	return 0;
}

