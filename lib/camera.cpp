/**
 * @file camera.cpp
 * @brief Camera class implementation
 * @date July 2023
 * @see https://learnopengl.com/Getting-started/Camera
 */

#include <learnopengl/camera.h>

Camera::Camera(
	glm::vec3 position, glm::vec3 up, 
	float yaw, float pitch
) : position(position), 
	front(glm::vec3(0.0f, 0.0f, -1.0f)),
	worldUp(up),
	yaw(yaw),
	pitch(pitch), 
	movementSpeed(SPEED),
	mouseSensitivity(SENSITIVITY),
	zoom(ZOOM) {
	updateCameraVectors();
}

Camera::Camera(
	float xPos, float yPos, float zPos,
	float xUp, float yUp, float zUp,
	float yaw, float pitch
) : position(glm::vec3(xPos, yPos, zPos)),
	front(glm::vec3(0.0f, 0.0f, -1.0f)),
	worldUp(glm::vec3(xUp, yUp, zUp)),
	yaw(yaw),
	pitch(pitch),
	movementSpeed(SPEED),
	mouseSensitivity(SENSITIVITY),
	zoom(ZOOM) {
	updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() {
	return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
	float velocity = movementSpeed * deltaTime;
	if (direction == FORWARD)
		position += front * velocity;
	if (direction == BACKWARD)
		position -= front * velocity;
	if (direction == LEFT)
		position -= right * velocity;
	if (direction == RIGHT)
		position += right * velocity;
}

void Camera::processMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch) {
	xOffset *= mouseSensitivity;
	yOffset *= mouseSensitivity;
	yaw += xOffset;
	pitch += yOffset;
	if (constrainPitch) {
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}
	updateCameraVectors();
}

void Camera::processMouseScroll(float yOffset) {
	zoom -= (float)yOffset;
	if (zoom < 1.0f)
		zoom = 1.0f;
	if (zoom > 45.0f)
		zoom = 45.0f;
}

void Camera::updateCameraVectors() {
	glm::vec3 cameraFront;
	cameraFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront.y = sin(glm::radians(pitch));
	cameraFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(cameraFront);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

glm::vec3 Camera::getPosition() { return position; }

glm::vec3 Camera::getFront() { return front; }

float Camera::getFOV() { return zoom; }