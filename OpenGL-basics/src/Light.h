#pragma once
#include "OPENGL.h"

class Light {
public:
	Light();
	Light(glm::vec3 position, glm::vec3 colour);

	void SetPosition(glm::vec3 position) { this->position = position; };
	void SetColour(glm::vec3 colour) { this->colour = colour; };
	
	glm::vec3 GetColour() { return colour; };
	glm::vec3 GetPosition() { return position; };

private:
	glm::vec3 position;
	glm::vec3 colour;
};