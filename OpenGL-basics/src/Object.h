#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Material.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>

class Texture;

class Object {

public:

	enum class DrawMode { TRIANGLES, TRIANGLE_STRIP };

	Object(float positions[], int positionsSize, unsigned int indices[], int indicesSize);
	~Object();

	void Bind(Shader* shader) const;
	void Unbind() const;

	void AddTexture(Texture* texture);
	void AddMaterial(Material* material);
	//void AddLight(Light* light);


	void SetTranslation(glm::vec3 t) { translation = t; };
	void SetRotation(glm::vec3 r) { rotation = r; };
	void SetScale(glm::vec3 s) { scale = s; };
	void SetDrawMode(DrawMode drawMode) { this->drawMode = drawMode; };

	glm::vec3 GetTranslation() const { return translation; };
	glm::vec3 GetRotation() const { return rotation; };
	glm::vec3 GetScale() const { return scale; };
	DrawMode GetDrawMode() { return drawMode; };
	unsigned int GetIndexCount() const { return ib.GetCount(); };

private:

	VertexArray va;
	IndexBuffer ib;
	VertexBuffer vb;
	VertexBufferLayout layout;

	glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);;
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);;
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);;

	//std::vector<Light*> lights;
	std::vector<Material*> materials;
	std::vector<Texture*> textures;
	DrawMode drawMode = DrawMode::TRIANGLES;

};