/**
 * @file multiple_lights.cpp
 * @brief Multiple lights: directional light, point lights, spotlight
 * @date Created: July 2023 | Last modified: January 2025
 * @see https://learnopengl.com/Lighting/Multiple-lights
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <iostream>

/**
 * Reads a provided texture into memory
 * @param path - the path to the texture
 * @param flipVertically - determines whether to flip the texture along the y-axis
 * @return The a texture ID
 */
unsigned int loadTexture(const char* path, bool flipVertically = false);

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
	const char* WINDOW_NAME = "Multiple Lights: Directional Light, Point Lights, & Spotlight";
	
	// Shaders
	const char* CUBE_VERTEX_SHADER = "src/2.lighting/6.multiple_lights/multiple_lights/cube.vs";
	const char* CUBE_FRAGMENT_SHADER = "src/2.lighting/6.multiple_lights/multiple_lights/cube.fs";
	const char* LIGHT_VERTEX_SHADER = "src/2.lighting/6.multiple_lights/multiple_lights/light.vs";
	const char* LIGHT_FRAGMENT_SHADER = "src/2.lighting/6.multiple_lights/multiple_lights/light.fs";

	// Textures
	const char* CONTAINER_DIFFUSE = "assets/textures/container2.png";
	const char* CONTAINER_SPECULAR = "assets/textures/container2_specular.png";	

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

	// establish cube vertex data
	float cubeVertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	// create VBO and VAOs
	unsigned int vbo, objectVao, lightVao;
	glGenBuffers(1, &vbo);

	// Copy vertex data to GPU and set vertex position attribute for object
	glGenVertexArrays(1, &objectVao);
	glBindVertexArray(objectVao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Set vertex position attribute for light source (ignore normals)
	glGenVertexArrays(1, &lightVao);
	glBindVertexArray(lightVao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Create shader objects
	Shader objectShader(CUBE_VERTEX_SHADER, CUBE_FRAGMENT_SHADER);
	Shader lightShader(LIGHT_VERTEX_SHADER, LIGHT_FRAGMENT_SHADER);

	// Load container texture/diffuse map
	unsigned int diffuseMap = loadTexture(CONTAINER_DIFFUSE);
	unsigned int specularMap = loadTexture(CONTAINER_SPECULAR);

	// Point light positions
	constexpr unsigned int NUM_POINT_LIGHTS = 4;
	glm::vec3 pointLightPositions[NUM_POINT_LIGHTS] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	// Container positions
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	// Set object material values (that remain constant)
	objectShader.use();
	objectShader.setInt("material.diffuse", 0);
	objectShader.setInt("material.specular", 1);
	objectShader.setFloat("material.shininess", 32.0f);

	// Set directional light values (that remain constant)
	objectShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	objectShader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
	objectShader.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
	objectShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);

	// Set point light values (that remain constant)
	for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
		objectShader.setVec3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
		objectShader.setVec3("pointLights[" + std::to_string(i) + "].ambient", 0.2f, 0.2f, 0.2f);
		objectShader.setVec3("pointLights[" + std::to_string(i) + "].diffuse", 0.5f, 0.5f, 0.5f);
		objectShader.setVec3("pointLights[" + std::to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);
		objectShader.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
		objectShader.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09f);
		objectShader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);
	}

	// Set spotlight values (that remain constant)
	objectShader.setFloat("spotLight.cutoff", glm::cos(glm::radians(12.5f)));
	objectShader.setFloat("spotLight.outerCutoff", glm::cos(glm::radians(17.5f)));
	objectShader.setVec3("spotLight.ambient", 0.2f, 0.2f, 0.2f);
	objectShader.setVec3("spotLight.diffuse", 0.5f, 0.5f, 0.5f);
	objectShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	objectShader.setFloat("spotLight.constant", 1.0f);
	objectShader.setFloat("spotLight.linear", 0.09f);
	objectShader.setFloat("spotLight.quadratic", 0.032f);

	// render loop
	while (!glfwWindowShouldClose(window)) {
		// compute change in time
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// process keyboard input
		processInput(window);

		// screen color
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		// clear color and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set spotlight values (that change)
		objectShader.use();
		objectShader.setVec3("spotLight.position", camera.getPosition());
		objectShader.setVec3("spotLight.direction", camera.getFront());

		// Bind textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		// render cube objects
		glm::mat4 model;
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.getFOV()), static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_WIDTH), 0.1f, 100.0f);
		objectShader.setMat4("view", view);
		objectShader.setMat4("projection", projection);
		objectShader.setVec3("viewPos", camera.getPosition());
		for (unsigned int i = 0; i < 10; i++) {
			model = glm::translate(glm::mat4(1.0f), cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			objectShader.setMat4("model", model);
			glBindVertexArray(objectVao);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// render point lights
		lightShader.use();
		for (int i = 0; i < NUM_POINT_LIGHTS; i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			lightShader.setMat4("model", model);
			lightShader.setMat4("view", view);
			lightShader.setMat4("projection", projection);
			glBindVertexArray(lightVao);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// deallocate all resources
	glDeleteVertexArrays(1, &objectVao);
	glDeleteVertexArrays(1, &lightVao);
	glDeleteBuffers(1, &vbo);
	glfwTerminate();
	return 0;
}

unsigned int loadTexture(const char* path, bool flipVertically) {
	unsigned int textureId;
	glGenTextures(1, &textureId);
	int width, height, numChannels;
	stbi_set_flip_vertically_on_load(flipVertically);
	unsigned char* data = stbi_load(path, &width, &height, &numChannels, 0);
	if (data) {
		GLenum format;
		switch (numChannels) {
			case 1:
			//default:
				format = GL_RED;
				break;
			case 3:
				format = GL_RGB;
				break;
			case 4: 
				format = GL_RGBA;
				break;
		}
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	} else {
		std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD\n" << path << std::endl;
	}
	stbi_image_free(data);
	return textureId;
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
