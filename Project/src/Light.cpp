#include "Light.h"

Light::Light()
{
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;

	direction = glm::vec3(0.0f, -1.0f, 0.0f);
	diffuseIntensity = 0.0f;
}

Light::Light(float red, float green, float blue, float ambientIntensity,float xDir,float yDir,float zDir,float diffuseIntensity)
{
	color = glm::vec3(red, green, blue);
	this->ambientIntensity = ambientIntensity;

	direction = glm::vec3(xDir, yDir, zDir);
	this->diffuseIntensity = diffuseIntensity;
}

void Light::UseLight(float ambientIntensityLocation, float ambientColourLocation,float diffuseIntensityLocation,float directionLocation)
{
	glUniform3f(ambientColourLocation, color.x, color.y, color.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);

	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

Light::~Light()
{
}
