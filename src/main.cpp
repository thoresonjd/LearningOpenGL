#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

// configurations
const int OPENGL_VERSION_MAJOR = 3;
const int OPENGL_VERSION_MINOR = 3;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const std::string WINDOW_NAME = "LearnOpenGL";
const float R = 0.5, G = 0.5, B = 0.5, A = 1.0;

/**
 * Process keyboard input
 * @param window - a GLFW window object
 */
void processInput(GLFWwindow* window);

/**
 * Handle window resizing
 * @param window - a GLFW window object
 * @param width - new width of resize
 * @param height - new height of resize
 */
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

int main(void) {

	// initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// create GLFW window object
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_NAME.c_str(), nullptr, nullptr);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);

	// set callbacks
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	
	// load OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	// render loop
	while (!glfwWindowShouldClose(window)) {
		
		// input
		processInput(window);

		// render commands
		glClearColor(R, G, B, A);
		glClear(GL_COLOR_BUFFER_BIT);

		// check and call events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}