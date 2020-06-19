#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>

class Texture;

class Object {

public:
	Object(float positions[], int positionsSize, unsigned int indices[], int indicesSize);
	~Object();

	void Bind(Shader* shader) const;

	void AddTexture(Texture* texture);

	void SetColour(glm::vec4 colour) { this->colour = colour; };
	void SetColour(float r, float g, float b, float a) { colour = glm::vec4(r, g, b, a); };
	void SetTranslation(glm::vec3 t) { translation = t; };
	glm::vec3 GetTranslation() const { return translation; };
	unsigned int GetIndexCount() const { return ib->GetCount(); };

private:
	VertexArray* va;
	IndexBuffer* ib;
	VertexBuffer* vb;
	VertexBufferLayout* layout;
	glm::vec3 translation;

	std::vector<Texture*> textures;
	glm::vec4 colour = glm::vec4(1,1,1,1);
};