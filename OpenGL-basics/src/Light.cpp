#include "Light.h"

Light::Light()
{
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 colour = glm::vec3(0, 0, 0);
}

Light::Light(glm::vec3 position, glm::vec3 colour)
{
	this->position = position;
	this->colour = colour;
}
