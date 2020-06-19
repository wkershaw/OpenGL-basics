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
	void SetRotation(glm::vec3 r) { rotation = r; };
	void SetScale(glm::vec3 s) { scale = s; };
		
	glm::vec3 GetTranslation() const { return translation; };
	glm::vec3 GetRotation() const { return rotation; };
	glm::vec3 GetScale() const { return scale; };
	unsigned int GetIndexCount() const { return ib->GetCount(); };

private:

	VertexArray* va;
	IndexBuffer* ib;
	VertexBuffer* vb;
	VertexBufferLayout* layout;
	glm::vec3 translation;
	glm::vec3 rotation;
	glm::vec3 scale;

	std::vector<Texture*> textures;
	glm::vec4 colour = glm::vec4(1,1,1,1);
};