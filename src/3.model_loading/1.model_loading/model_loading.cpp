/**
 * @file model_loading.cpp
 * @brief Loading a model
 * @date Created: July 2023 | Last modified: February 2025
 * @see https://learnopengl.com/Model-Loading/Model
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <learnopengl/shader_g.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>
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
void mouseCallback(GLFWwindow* window, double posX, double posY);

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

namespace {
	// GLFW
	constexpr int OPENGL_VERSION_MAJOR = 3;
	constexpr int OPENGL_VERSION_MINOR = 3;
	constexpr int SCREEN_WIDTH = 800;
	constexpr int SCREEN_HEIGHT = 600;
	const char* WINDOW_NAME = "Model Loading";
	
	// Shaders
	const char* MODEL_VERTEX_SHADER = "src/3.model_loading/1.model_loading/model.vs";
	const char* MODEL_FRAGMENT_SHADER = "src/3.model_loading/1.model_loading/model.fs";
	const char* LIGHT_VERTEX_SHADER = "src/3.model_loading/1.model_loading/light.vs";
	const char* LIGHT_FRAGMENT_SHADER = "src/3.model_loading/1.model_loading/light.fs";

	// Model
	const char* MODEL_PATH = "assets/models/backpack/backpack.obj";

	// Camera
	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

	// Time
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	// Mouse
	bool isFirstMouse = true;
	float lastMouseX = static_cast<float>(SCREEN_WIDTH) / 2.0f;
	float lastMouseY = static_cast<float>(SCREEN_HEIGHT) / 2.0f;
}

int main(void) {
	// initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create GLFW window object
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_NAME, nullptr, nullptr);
	if (!window) {
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// initialize GLAD: load OpenGL function pointers
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		std::cout << "Failed to initialize GLAD\n";
		glfwTerminate();
		return -1;
	}

	// enable OpenGL features
	glEnable(GL_DEPTH_TEST);

	// create light source object
	float lightVertices[] = {
		0.5f, 0.5f, 0.5f,	 // 0: right top front
		0.5f, 0.5f, -0.5f,   // 1: right top back
		0.5f, -0.5f, 0.5f,   // 2: right bottom front
		0.5f, -0.5f, -0.5f,  // 3: right bottom back
		-0.5f, 0.5f, 0.5f,   // 4: left top front
		-0.5f, 0.5f, -0.5f,  // 5: left top back
		-0.5f, -0.5f, 0.5f,  // 6: left bottom front
		-0.5f, -0.5f, -0.5f, // 7: left bottom back
	};

	unsigned int lightIndices[] = {
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

	// create VBO and VAOs
	unsigned int lightVbo, lightVao, lightEbo;
	glGenBuffers(1, &lightVbo);

	// Copy vertex data to GPU and set vertex position attribute for object
	glGenVertexArrays(1, &lightVao);
	glBindVertexArray(lightVao);
	glBindBuffer(GL_ARRAY_BUFFER, lightVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertices), lightVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Set vertex position attribute for light source
	glGenVertexArrays(1, &lightVao);
	glBindVertexArray(lightVao);
	glBindBuffer(GL_ARRAY_BUFFER, lightVbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	// copy indices into element buffer
	glGenBuffers(1, &lightEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lightIndices), lightIndices, GL_STATIC_DRAW);

	// unbind VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// unbind EBO after VAO to keep EBO data bound to VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// create model object
	stbi_set_flip_vertically_on_load(true);
	Model object(MODEL_PATH);

	// Create shader objects
	Shader objectShader(MODEL_VERTEX_SHADER, MODEL_FRAGMENT_SHADER);
	Shader lightShader(LIGHT_VERTEX_SHADER, LIGHT_FRAGMENT_SHADER);

	// render loop
	while (!glfwWindowShouldClose(window)) {
		// per-frame time logic; compute change in time
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// process keyboard input
		processInput(window);

		// screen color
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		// clear color and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render the light source; full revolution around the model every four seconds
		glm::vec3 lightPos(0.0f);
		glm::mat4 model(1.0f);
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.getFOV()), static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_WIDTH), 0.1f, 100.0f);
		lightShader.use();
		lightPos.x = 5.0f * sin(currentFrame * glm::radians(90.0f));
		lightPos.z = 5.0f * cos(currentFrame * glm::radians(90.0f));
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.5f));
		lightShader.setMat4("model", model);
		lightShader.setMat4("view", view);
		lightShader.setMat4("projection", projection);
		glBindVertexArray(lightVao);
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(float), GL_UNSIGNED_INT, 0);

		// render loaded model
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
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
		objectShader.setMat4("model", model);
		objectShader.setMat4("view", view);
		objectShader.setMat4("projection", projection);
		object.draw(objectShader);

		// swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// deallocate all resources
	glDeleteVertexArrays(1, &lightVao);
	glDeleteBuffers(1, &lightVbo);
	glDeleteBuffers(1, &lightEbo);
	glfwTerminate();
	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xPosition, double yPosition) {
	float xPos = static_cast<float>(xPosition);
	float yPos = static_cast<float>(yPosition);

	if (isFirstMouse) {
		lastMouseX = xPos;
		lastMouseY = yPos;
		isFirstMouse = false;
	}

	float xOffset = xPos - lastMouseX;
	float yOffset = lastMouseY - yPos; // reversed since y-coordinates range from bottom to top
	lastMouseX = xPos;
	lastMouseY = yPos;
	camera.processMouseMovement(xOffset, yOffset);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	camera.processMouseScroll(static_cast<float>(yOffset));
}

void processInput(GLFWwindow* window) {
	// window close / exit program
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// camera movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(CameraMovement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(CameraMovement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(CameraMovement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(CameraMovement::RIGHT, deltaTime);
}