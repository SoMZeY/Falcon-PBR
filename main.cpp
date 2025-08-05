#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <openglErrorReporting.h>
#include <vertexBuffer.h>
#include <vertexArray.h>
#include <vertexBufferLayout.h>
#include <indexBuffer.h>
#include <shader.h>
#include <fpsCameraController.h>
#include <gltfScene.h>

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

	// Triangle vertex buffer data
	float verticesTriangle[] = {
		//  COORDINATES         COLORS
		-0.8f, -0.8f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.8f, -0.8f, 0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f,  0.8f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	unsigned int indexesTriangle[] = {
		0, 1, 2
	};

	// Load Damaged Helmet
	GLTFScene damagedHelmet(std::string(RESOURCES_PATH) + "models/gltf/DamagedHelmet.glb");
	// Create shaders and shader program for the damagedHelmet
	Shader damagedHelmetProgram((std::string(RESOURCES_PATH) + "shaders/vertexGltf.vert").c_str(),
		(std::string(RESOURCES_PATH) + "shaders/fragmentGltf.frag").c_str());
	
	// Create and bind vbo
	VertexBuffer vbo(verticesTriangle, sizeof(verticesTriangle), GL_STATIC_DRAW);
	vbo.Bind();

	// Create buffer layout and push coord and color layout elements
	VertexBufferLayout vbl;
	vbl.Push<float>(3);
	vbl.Push<float>(3);

	// Create and bind vao, and call add buffer
	VertexArray vao;
	vao.AddBuffer(vbo, vbl);
	vao.Bind();

	// Create and bind ibo, thus will be binded to the vao
	IndexBuffer ibo(indexesTriangle, 3, GL_STATIC_DRAW);
	ibo.Bind();

	// Create shaders and shader program
	Shader shaderProgram((std::string(RESOURCES_PATH) + "shaders/vertexShader.vert").c_str(),
						 (std::string(RESOURCES_PATH) + "shaders/fragmentShader.frag").c_str());

	// Unbind vbo, vao, ibo
	vbo.Unbind();
	vao.Unbind();
	ibo.Unbind();


	// Camera related
	Camera camera(45.0f, 640.0f / 480.0f, 0.01f, 100.0f);
	camera.SetPosition({ 0.0f, 0.0f, 2.0f });
	FpsCameraController controller(&camera);
	g_controller = &controller;

	// Set input callbacks
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetKeyCallback(window, key_callback);


	// Delta time 
	float lastFrameTime = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		if (windowClose) break;
		// Delta time logic
		float currentTime = static_cast<float>(glfwGetTime());
		float deltaTime = currentTime - lastFrameTime;
		lastFrameTime = currentTime;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		controller.Update(deltaTime);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = camera.GetProjectionMatrix();
		glm::mat4 model = glm::mat4(1.0f);
		damagedHelmetProgram.use();
		glm::mat4 mvp = projection * view * model;
		damagedHelmetProgram.setMat4("u_MVP", mvp);
		damagedHelmet.Draw(damagedHelmetProgram);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);

	glfwTerminate();

	return 0;
}

