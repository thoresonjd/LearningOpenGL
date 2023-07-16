/**
 * @file camera.h
 * @brief Camera class
 * @date July 2023
 * @see https://learnopengl.com/Getting-started/Camera
 */

#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

// abstraction to stay away from windowing system specific input methods
enum class CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 5.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera {
public:
	// vector and scalar constructors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
	Camera(float xPos, float yPos, float zPos, float xUp, float yUp, float zUp, float yaw, float pitch);
	glm::mat4 getViewMatrix();
	void processKeyboard(CameraMovement direction, float deltaTime);
	void processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);
	void processMouseScroll(float yOffset);
	glm::vec3 getPosition();
	glm::vec3 getFront();
	float getFOV();
private:
	// camera attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;
	// Euler angles
	float yaw;
	float pitch;
	// camera options
	float movementSpeed;
	float mouseSensitivity;
	float zoom;
	void updateCameraVectors();
};