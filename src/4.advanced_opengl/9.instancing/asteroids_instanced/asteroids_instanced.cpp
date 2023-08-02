/**
 * @file asteroids_instanced.cpp
 * @brief Utilizing instancing to render multiple asteroids
 * @date August 2023
 * @see https://learnopengl.com/Advanced-OpenGL/Instancing
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
const char* WINDOW_NAME = "Instancing: asteroids instanced";
// shaders
const char* PLANET_VERT_SHADER = "src/4.advanced_opengl/9.instancing/asteroids_instanced/planet.vs";
const char* PLANET_FRAG_SHADER = "src/4.advanced_opengl/9.instancing/asteroids_instanced/planet.fs";
const char* ASTEROID_VERT_SHADER = "src/4.advanced_opengl/9.instancing/asteroids_instanced/asteroid.vs";
const char* ASTEROID_FRAG_SHADER = "src/4.advanced_opengl/9.instancing/asteroids_instanced/asteroid.fs";
// model
const char* ASTEROID = "assets/models/rock/rock.obj";
const char* PLANET = "assets/models/planet/planet.obj";
// camera
Camera camera(glm::vec3(0.0f, 15.0f, 100.0f));
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

	// create shader program objects
	Shader planetShader(PLANET_VERT_SHADER, PLANET_FRAG_SHADER);
	Shader asteroidShader(ASTEROID_VERT_SHADER, ASTEROID_FRAG_SHADER);

	// load models
	stbi_set_flip_vertically_on_load(true);
	Model asteroid(ASTEROID);
	Model planet(PLANET);

	// asteroid placement
	const unsigned int NUM_ASTEROIDS = 10000;
	glm::mat4* modelMatrices = new glm::mat4[NUM_ASTEROIDS];
	srand(static_cast<unsigned int>(glfwGetTime())); // initialize random seed
	float radius = 75.0f;
	float offset = 10.0f;
	for (unsigned int i = 0; i < NUM_ASTEROIDS; i++) {
		glm::mat4 model = glm::mat4(1.0f);
		// 1. translation: displace along circle with radius in range [-offset, offset]
		float angle = (float)i / (float)NUM_ASTEROIDS * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));
		
		// 2. scale: scale between 0.05 and 0.25f
		float scale = static_cast<float>(rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		float rotAngle = static_cast<float>(rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. now add to list of matrices
		modelMatrices[i] = model;
	}

	// configure instance array
	unsigned int instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, NUM_ASTEROIDS * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
	for (unsigned int i = 0; i < asteroid.getNumMeshes(); i++) {
		unsigned int instanceVAO = asteroid.getMeshVAO(i);
		glBindVertexArray(instanceVAO);
		// vertex attributes
		// maximum amount of data allowed for a vertex attribute is equal to a vec4
		// mat4 is basically 4 vec4s, we must reserve 4 vertex attributes for this specific matrix
		std::size_t vec4Size = sizeof(glm::vec4);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)vec4Size);
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glBindVertexArray(0);
	}

	// render loop
	while (!glfwWindowShouldClose(window)) {
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// process input
		processInput(window);

		// set color
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		// clear buffer bits so information does not overlap
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// view and model matrices
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.getFOV()), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 1000.0f);
		
		// render planet
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		planetShader.use();
		planetShader.setMat4("model", model);
		planetShader.setMat4("view", view);
		planetShader.setMat4("projection", projection);
		planet.draw(planetShader);
		
		// render asteroids
		asteroidShader.use();
		asteroidShader.setMat4("view", view);
		asteroidShader.setMat4("projection", projection);
		// asteroid texture needs to be activated, otherwise the planet texture will display on the asteroid
		// asteroid texture currently isn't loading
		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, asteroid.getTextureID(0));*/
		for (unsigned int i = 0; i < asteroid.getNumMeshes(); i++) {
			glBindVertexArray(asteroid.getMeshVAO(i));
			glDrawElementsInstanced(GL_TRIANGLES, asteroid.getMeshNumIndices(i), GL_UNSIGNED_INT, 0, NUM_ASTEROIDS);
			glBindVertexArray(0);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete[] modelMatrices;
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