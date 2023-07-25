/**
 * @file advanced_glsl_ubo.cpp
 * @brief Using a uniform buffer object
 * @date July 2023
 * @see https://learnopengl.com/Advanced-OpenGL/Advanced-GLSL
 */

// OpenGL implementation
#include <glad/glad.h>
#include <GLFW/glfw3.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// Custom libs
#include <learnopengl/shader_m.h>
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
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char* WINDOW_NAME = "Advanced GLSL: Uniform Buffer Object";
// shaders
const char* VERT_SHADER = "src/4.advanced_opengl/7.advanced_glsl/advanced_glsl_ubo.vs";
const char* RED_FRAG_SHADER = "src/4.advanced_opengl/7.advanced_glsl/red.fs";
const char* YELLOW_FRAG_SHADER = "src/4.advanced_opengl/7.advanced_glsl/yellow.fs";
const char* GREEN_FRAG_SHADER = "src/4.advanced_opengl/7.advanced_glsl/green.fs";
const char* BLUE_FRAG_SHADER = "src/4.advanced_opengl/7.advanced_glsl/blue.fs";
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
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	// since each triangle's vertices are specified in counter-clockwise order,
	// when facing away from the viewer (back facing), these faces will be culled (discarded)
	// because they will thus be in clockwise order instead. setting the front face to
	// clockwise reverses this effect, culling the front faces instead
	// glFrontFace(GL_CW);

	// create shader program objects
	Shader redShader(VERT_SHADER, RED_FRAG_SHADER);
	Shader yellowShader(VERT_SHADER, YELLOW_FRAG_SHADER);
	Shader greenShader(VERT_SHADER, GREEN_FRAG_SHADER);
	Shader blueShader(VERT_SHADER, BLUE_FRAG_SHADER);

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

	// set uniform block of vertex shaders to binding point 0
	unsigned int uniformBlockIndexRed = glGetUniformBlockIndex(redShader.getID(), "Matrices");
	unsigned int uniformBlockIndexYellow = glGetUniformBlockIndex(yellowShader.getID(), "Matrices");
	unsigned int uniformBlockIndexGreen = glGetUniformBlockIndex(greenShader.getID(), "Matrices");
	unsigned int uniformBlockIndexBlue = glGetUniformBlockIndex(blueShader.getID(), "Matrices");
	glUniformBlockBinding(redShader.getID(), uniformBlockIndexRed, 0);
	glUniformBlockBinding(yellowShader.getID(), uniformBlockIndexYellow, 0);
	glUniformBlockBinding(greenShader.getID(), uniformBlockIndexGreen, 0);
	glUniformBlockBinding(blueShader.getID(), uniformBlockIndexBlue, 0);

	// create Uniform Buffer Object (UBO)
	unsigned int matricesUBO;
	glGenBuffers(1, &matricesUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, matricesUBO, 0, 2 * sizeof(glm::mat4));

	// projection matrix
	glm::mat4 projection = glm::perspective(glm::radians(FOV), aspectRatio, 0.1f, 100.0f);
	glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// render loop
	while (!glfwWindowShouldClose(window)) {
		// per-frame time logic
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// process input
		processInput(window);

		// set color
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		// clear buffer bits so information does not overla
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// view matrix
		glm::mat4 view = camera.getViewMatrix();
		glBindBuffer(GL_UNIFORM_BUFFER, matricesUBO);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		// bind cube VAO
		glBindVertexArray(cubeVAO);

		// red cube
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.75f, 0.75f, 0.0f));
		redShader.use();
		redShader.setMat4("model", model);
		redShader.setMat4("view", view);
		redShader.setMat4("projection", projection);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// yellow cube
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-0.75f, -0.75f, 0.0f));
		yellowShader.use();
		yellowShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// green cube
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.75f, 0.75f, 0.0f));
		greenShader.use();
		greenShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// blue cube
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.75f, -0.75f, 0.0f));
		blueShader.use();
		blueShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// unbind cube VAO
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