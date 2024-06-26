#include "Camera.h"

Camera::Camera()
{

}

Camera::Camera(glm::vec3 position,glm::vec3 up,float yaw,float pitch,float movementSpeed,float turnSpeed)
{
	this->position = position;
	this->worldUp = up;
	this->yaw = yaw;
	this->pitch = pitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	this->movementSpeed = movementSpeed;
	this->turnSpeed = turnSpeed;

	update();
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

void Camera::keyControl(bool* keys,float deltaTime)
{
	if (keys[GLFW_KEY_W])
	{
		position += front * movementSpeed * deltaTime;
	}
	if (keys[GLFW_KEY_S])
	{
		position -= front * movementSpeed * deltaTime;
	}
	if (keys[GLFW_KEY_A])
	{
		position -= right * movementSpeed * deltaTime;
	}
	if (keys[GLFW_KEY_D])
	{
		position += right * movementSpeed * deltaTime;
	}
}

void Camera::mouseControl(float xChange, float yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;
	
	yaw += xChange;
	pitch += yChange;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update();
}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

Camera::~Camera()
{
}
