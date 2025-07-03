#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <openglErrorReporting.h>
#include <vertexBuffer.h>
#include <vertexArray.h>
#include <vertexBufferLayout.h>
#include <indexBuffer.h>
#include <shader.h>

#include <iostream>


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

	GLFWwindow *window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
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

	enableReportGlErrors();

	// Enable culling and depth
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);

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

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		// Bind vbo
		vbo.Bind();

		// Bind vao
		vao.Bind();

		// Use program
		shaderProgram.use();

		// Do a draw triangles call
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();

	return 0;
}

