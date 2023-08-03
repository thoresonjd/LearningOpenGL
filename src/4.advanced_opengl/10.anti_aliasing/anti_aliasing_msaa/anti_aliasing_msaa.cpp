/**
 * @file anti_aliasing_msaa.cpp
 * @brief Multisample Anti-aliasing (MSAA)
 * @date August 2023
 * @see https://learnopengl.com/Advanced-OpenGL/Anti-Aliasing
 */

// OpenGL implementation
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// Custom libs
#include <learnopengl/shader_g.h>
#include <learnopengl/camera.h>
// C++ libs
#include <iostream>

/**
 * Handle window resizing
 * @param window - a GLFW window object
 * @param width - new width of resize
 * @param height - new height of resize
 */
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

/**
 * Handle mouse movement
 * @param window - a GLFW window object
 * @param xPos - x position of the mouse
 * @param yPos - y position of the mouse
 */
void mouseCallback(GLFWwindow* window, double xPos, double yPos);

/**
 * Process keyboard input
 * @param window - a GLFW window object
 */
void processInput(GLFWwindow* window);

// OpenGL configurations
const int OPENGL_VERSION_MAJOR = 3;
const int OPENGL_VERSION_MINOR = 3;
const int MSAA_NUM_SAMPLES = 4;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char* WINDOW_NAME = "Multisample Anti-aliasing (MSAA)";
// shaders
const char* VERT_SHADER = "src/4.advanced_opengl/10.anti_aliasing/anti_aliasing_msaa/anti_aliasing.vs";
const char* FRAG_SHADER = "src/4.advanced_opengl/10.anti_aliasing/anti_aliasing_msaa/anti_aliasing.fs";
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
const float FOV = 45.0f;
float aspectRatio = SCREEN_WIDTH / SCREEN_HEIGHT;
float lastX = SCREEN_WIDTH / 2;
float lastY = SCREEN_HEIGHT / 2;
bool firstMouse = true;
// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main(void) {
	// initialize GLFW and create window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, MSAA_NUM_SAMPLES); // tell GLFW to use multisampling with 4 samples
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_NAME, nullptr, nullptr);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	// configure global OpenGL state
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE); // enabled by default, but good practice
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	// since each triangle's vertices are specified in counter-clockwise order,
	// when facing away from the viewer (back facing), these faces will be culled (discarded)
	// because they will thus be in clockwise order instead. setting the front face to
	// clockwise reverses this effect, culling the front faces instead
	// glFrontFace(GL_CW);

	// create shader program object
	Shader shader(VERT_SHADER, FRAG_SHADER);

	/*
	Remember: to specify vertices in a counter-clockwise winding order you need to visualize the triangle
	as if you're in front of the triangle and from that point of view, is where you set their order.

	To define the order of a triangle on the right side of the cube for example, you'd imagine yourself looking
	straight at the right side of the cube, and then visualize the triangle and make sure their order is specified
	in a counter-clockwise order. This takes some practice, but try visualizing this yourself and see that this
	is correct.
	*/
	float cubeVertices[] = {
		// Back face
		-0.5f, -0.5f, -0.5f,  
		 0.5f,  0.5f, -0.5f,  
		 0.5f, -0.5f, -0.5f,          
		 0.5f,  0.5f, -0.5f,  
		-0.5f, -0.5f, -0.5f,  
		-0.5f,  0.5f, -0.5f,  
		// Front face
		-0.5f, -0.5f,  0.5f,  
		 0.5f, -0.5f,  0.5f,  
		 0.5f,  0.5f,  0.5f,  
		 0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f,  0.5f,  
		-0.5f, -0.5f,  0.5f,  
		// Left face
		-0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f, -0.5f,  
		-0.5f, -0.5f, -0.5f,  
		-0.5f, -0.5f, -0.5f,  
		-0.5f, -0.5f,  0.5f,  
		-0.5f,  0.5f,  0.5f,  
		// Right face
		 0.5f,  0.5f,  0.5f,  
		 0.5f, -0.5f, -0.5f,  
		 0.5f,  0.5f, -0.5f,       
		 0.5f, -0.5f, -0.5f,  
		 0.5f,  0.5f,  0.5f,  
		 0.5f, -0.5f,  0.5f,     
		 // Bottom face
		 -0.5f, -0.5f, -0.5f, 
		  0.5f, -0.5f, -0.5f, 
		  0.5f, -0.5f,  0.5f, 
		  0.5f, -0.5f,  0.5f, 
		 -0.5f, -0.5f,  0.5f, 
		 -0.5f, -0.5f, -0.5f, 
		 // Top face
		 -0.5f,  0.5f, -0.5f, 
		  0.5f,  0.5f,  0.5f, 
		  0.5f,  0.5f, -0.5f,   
		  0.5f,  0.5f,  0.5f, 
		 -0.5f,  0.5f, -0.5f, 
		 -0.5f,  0.5f,  0.5f   
	};

	// cube VAO
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	// render loop
	while (!glfwWindowShouldClose(window)) {
		// per-frame time logic
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// process input
		processInput(window);

		// render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render cube
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.getFOV()), aspectRatio, 0.1f, 100.0f);
		glBindVertexArray(cubeVAO);
		shader.use();
		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optionally deallocate all resources
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);

	glfwTerminate();
	return EXIT_SUCCESS;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xPos, double yPos) {
	float xPosition = static_cast<float>(xPos);
	float yPosition = static_cast<float>(yPos);
	if (firstMouse) {
		lastX = xPosition;
		lastY = yPosition;
		firstMouse = false;
	}
	float xOffset = xPosition - lastX;
	float yOffset = lastY - yPosition; // reversed since y-coordinates range from top to bottom
	lastX = xPosition;
	lastY = yPosition;
	camera.processMouseMovement(xOffset, yOffset);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(CameraMovement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(CameraMovement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(CameraMovement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(CameraMovement::RIGHT, deltaTime);
}