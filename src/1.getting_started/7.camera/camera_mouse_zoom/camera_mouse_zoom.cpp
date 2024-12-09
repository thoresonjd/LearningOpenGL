/**
 * @file camera_mouse_zoom.cpp
 * @brief Camera movement controlled via keyboard and mouse input
 * @date Created: July 2023 | Last modified: November 2024
 * @see https://learnopengl.com/Getting-started/Camera
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <learnopengl/shader_m.h>
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
	const char* WINDOW_NAME = "Camera: Mouse";
	
	// Textures
	const char* CONTAINER_TEX_PATH = "assets/textures/container.jpg";
	const char* AWESOMEFACE_TEX_PATH = "assets/textures/awesomeface.png";
	
	// Shaders
	const char* VERTEX_SHADER_PATH = "src/1.getting_started/7.camera/camera_mouse_zoom/shader.vs";
	const char* FRAGMENT_SHADER_PATH = "src/1.getting_started/7.camera/camera_mouse_zoom/shader.fs";

	// Camera
	glm::vec3 cameraPosition(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);
	float pitch = 0.0f;
	float yaw = -90.0f;
	float fieldOfView = 45.0f;

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
	float vertices[] = {
		// positions		  // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	// create vertex buffer object and vertex array object
	unsigned int vbo, vao;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	// bind VAO before configuring vertex data so data is bound to VAO
	glBindVertexArray(vao);

	// copy vertices array into vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// tell OpenGL how to interpret the vertex data via setting vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// unbind VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// create texture objects
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glGenTextures(1, &texture2);

	// set texture wrapping/filtering options for the texture objects
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load textures
	stbi_set_flip_vertically_on_load(true);
	int width, height, numChannels;
	unsigned char* data = stbi_load(CONTAINER_TEX_PATH, &width, &height, &numChannels, 0);
	if (data) {
		glBindTexture(GL_TEXTURE_2D, texture1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture " << CONTAINER_TEX_PATH << std::endl;
	}
	stbi_image_free(data);
	data = stbi_load(AWESOMEFACE_TEX_PATH, &width, &height, &numChannels, 0);
	if (data) {
		glBindTexture(GL_TEXTURE_2D, texture2);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << AWESOMEFACE_TEX_PATH << std::endl;
	}
	stbi_image_free(data);

	// create shader object
	Shader shader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

	// notify OpenGL which texture unit each shader sampler belongs to
	shader.use();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);

	// set up cube positions
	glm::vec3 cubePositions[] = {
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(2.0f, 5.0f,-15.0f),
			glm::vec3(-1.5f,-2.2f,-2.5f),
			glm::vec3(-3.8f,-2.0f,-12.3f),
			glm::vec3(2.4f,-0.4f,-3.5f),
			glm::vec3(-1.7f, 3.0f,-7.5f),
			glm::vec3(1.3f,-2.0f,-2.5f),
			glm::vec3(1.5f, 2.0f,-2.5f),
			glm::vec3(1.5f, 0.2f,-1.5f),
			glm::vec3(-1.3f, 1.0f,-1.5f)
	};

	// render loop
	while (!glfwWindowShouldClose(window)) {
		// compute change in time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// process keyboard input
		processInput(window);

		// screen color
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

		// clear color and depth buffers
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// camera/view/LookAt transformation
		glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
		glm::mat4 projection = glm::perspective(glm::radians(fieldOfView), static_cast<float>(SCREEN_WIDTH) / SCREEN_HEIGHT, 0.1f, 100.0f);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		// draw textured cubes
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glBindVertexArray(vao);
		for (int i = 0; i < 10; i++) {
			glm::mat4 model = glm::translate(glm::mat4(1.0f), cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		// swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// deallocate all resources
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
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

	constexpr float SENSITIVITY = 0.05f;
	xOffset *= SENSITIVITY;
	yOffset *= SENSITIVITY;

	yaw += xOffset;
	pitch += yOffset;

	// constrain pitch (vertical angle)
	constexpr float PITCH_MAX = 89.0f;
	constexpr float PITCH_MIN = -89.0f;
	if (pitch > PITCH_MAX)
		pitch = PITCH_MAX;
	if (pitch < PITCH_MIN)
		pitch = PITCH_MIN;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	constexpr float FOV_MAX = 45.0f;
	constexpr float FOV_MIN = 1.0f;
	fieldOfView -= static_cast<float>(yOffset);
	if (fieldOfView < FOV_MIN)
		fieldOfView = FOV_MIN;
	if (fieldOfView > FOV_MAX)
		fieldOfView = FOV_MAX;
}

void processInput(GLFWwindow* window) {
	// window close / exit program
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// camera movement
	constexpr float UNITS_PER_SECOND = 5.0f;
	const float CAMERA_SPEED = UNITS_PER_SECOND * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPosition += CAMERA_SPEED * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPosition -= CAMERA_SPEED * glm::normalize(glm::cross(cameraFront, cameraUp)); // create cameraRight via cross product
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPosition -= CAMERA_SPEED * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPosition += CAMERA_SPEED * glm::normalize(glm::cross(cameraFront, cameraUp));
}