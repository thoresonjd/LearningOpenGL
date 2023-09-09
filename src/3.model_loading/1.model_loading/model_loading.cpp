/**
 * @file model_loading.cpp
 * @brief Loading a model
 * @date July 2023
 * @see https://learnopengl.com/Model-Loading/Model
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
#include <learnopengl/model.h>
// Image loading
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
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
 * Handle mouse scrolling
 * @param window - a GLFW window
 * @param xOffset - amount scrolled along the x-axis
 * @param yOffset - amount scrolled along the y-axis
 */
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

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
const char* WINDOW_NAME = "Model loading";
// shaders
const char* MODEL_VERT_SHADER = "src/3.model_loading/1.model_loading/model.vs";
const char* MODEL_FRAG_SHADER = "src/3.model_loading/1.model_loading/model.fs";
const char* LIGHT_VERT_SHADER = "src/3.model_loading/1.model_loading/light.vs";
const char* LIGHT_FRAG_SHADER = "src/3.model_loading/1.model_loading/light.fs";
// model
const char* MODEL = "assets/models/backpack/backpack.obj";
// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
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
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	// configure global OpenGL state
	glEnable(GL_DEPTH_TEST);

	// tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
	stbi_set_flip_vertically_on_load(true);

	// create model object
	Model object(MODEL);
	
	// create shader programs
	Shader objectShader(MODEL_VERT_SHADER, MODEL_FRAG_SHADER);
	Shader lightShader(LIGHT_VERT_SHADER, LIGHT_FRAG_SHADER);

	// create light source object
	float vertices[] = {
		0.5f, 0.5f, 0.5f,	 // 0: right top front
		0.5f, 0.5f, -0.5f,   // 1: right top back
		0.5f, -0.5f, 0.5f,   // 2: right bottom front
		0.5f, -0.5f, -0.5f,  // 3: right bottom back
		-0.5f, 0.5f, 0.5f,   // 4: left top front
		-0.5f, 0.5f, -0.5f,  // 5: left top back
		-0.5f, -0.5f, 0.5f,  // 6: left bottom front
		-0.5f, -0.5f, -0.5f, // 7: left bottom back
	};

	unsigned int indices[] = {
		0, 1, 4, // top
		1, 4, 5,
		1, 4, 6, // front
		1, 2, 6,
		1, 3, 5, // back
		3, 5, 7,
		2, 3, 6, // bottom
		3, 6, 7,
		0, 1, 2, // right
		1, 2, 3,
		4, 5, 6, // left
		5, 6, 7
	};

	unsigned int lightVBO, lightVAO, lightEBO;
	glGenVertexArrays(1, &lightVAO);
	glGenBuffers(1, &lightVBO);
	glGenBuffers(1, &lightEBO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
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

		// set color
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		// clear buffer bits so information does not overlap frames
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// view/projection transformations
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.getFOV()), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

		// render the light source
		lightShader.use();
		glm::mat4 model = glm::mat4(1.0f);
		glm::vec3 lightPos(0.0f);
		// full revolution around the model every four seconds
		lightPos.x = 5.0f * sin(currentFrame * glm::radians(90.0f));
		lightPos.z = 5.0f * cos(currentFrame * glm::radians(90.0f));
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		lightShader.setMat4("model", model);
		lightShader.setMat4("view", view);
		lightShader.setMat4("projection", projection);
		glBindVertexArray(lightVAO);
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(float), GL_UNSIGNED_INT, 0);

		// enable object shader, configure lighting, set transformation uniforms
		objectShader.use();
		objectShader.setVec3("viewPos", camera.getPosition());
		objectShader.setFloat("material.shininess", 64.0f);
		objectShader.setVec3("light.position", lightPos);
		objectShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		objectShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		objectShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		objectShader.setFloat("light.constant", 1.0f);
		objectShader.setFloat("light.linear", 0.09f);
		objectShader.setFloat("light.quadratic", 0.032f);
		objectShader.setMat4("view", view);
		objectShader.setMat4("projection", projection);

		// render the loaded model
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		objectShader.setMat4("model", model);
		object.draw(objectShader);

		// swap buffers and poll events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

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

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	camera.processMouseScroll(static_cast<float>(yOffset));
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