/**
 * @file shaders_uniform.cpp
 * @brief Using a uniform to render a color-changing triangle
 * @date Created: July 2023 | Last modified: November 2024
 * @see https://learnopengl.com/Getting-started/Shaders
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

/**
 * Handle window resizing
 * @param window - a GLFW window object
 * @param width - new width of resize
 * @param height - new height of resize
 */
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

/**
 * Process keyboard input
 * @param window - a GLFW window object
 */
void processInput(GLFWwindow* window);

namespace {
	// configurations
	const int OPENGL_VERSION_MAJOR = 3;
	const int OPENGL_VERSION_MINOR = 3;
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;
	const char* WINDOW_NAME = "Shaders: Uniform";
	
	const char* VERTEX_SHADER_SOURCE = R"(
		#version 330 core
		layout (location = 0) in vec3 aPos;
		void main() {
			gl_Position = vec4(aPos, 1.0);
		}
	)";

	const char* FRAGMENT_SHADER_SOURCE = R"(
		#version 330 core
		out vec4 fragColor;
		uniform vec4 vertexColor;
		void main() {
			fragColor = vertexColor;
		}
	)";
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
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// initialize GLAD: load OpenGL function pointers
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	// create vertex shader
	int success;
	char infoLog[512];
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &VERTEX_SHADER_SOURCE, nullptr);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// create fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &FRAGMENT_SHADER_SOURCE, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// create shader program
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// establish triangle vertex data
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	// unbind VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// render loop
	while (!glfwWindowShouldClose(window)) {
		// input
		processInput(window);

		// screen color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// activate shader program
		glUseProgram(shaderProgram);

		// update uniform color
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "vertexColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		// draw triangle
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// deallocate all resources
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteProgram(shaderProgram);
	glfwTerminate();
	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
