#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

class Light {
private:
	glm::vec3 color;
	float ambientIntensity;

	glm::vec3 direction;
	float diffuseIntensity;
public:
	Light();
	Light(float red, float green, float blue, float ambientIntensity, float xDir, float ydir, float zDir, float diffuseIntensity);

	void UseLight(float ambientIntensityLocation, float ambientColourLocation, float diffuseIntensityLocation,float directionLocation);


	~Light();
};