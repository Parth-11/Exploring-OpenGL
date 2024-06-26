#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\glm.hpp>

class Camera {
private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	float yaw;
	float pitch;

	float movementSpeed;
	float turnSpeed;

	void update();

public:
	Camera();
	Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float movementSpeed, float turnSpeed);
	~Camera();

	glm::mat4 calculateViewMatrix();
	void keyControl(bool* keys,float deltaTime);
	void mouseControl(float xChange, float yChange);
};