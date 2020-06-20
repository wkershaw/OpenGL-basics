#include "Object.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

Object::Object(float positions[], int positionsSize, unsigned int indices[], int indicesSize) {
	textures = *new std::vector<Texture*>();
	materials = *new std::vector<Material*>();
	va = new VertexArray();
	vb = new VertexBuffer(positions, positionsSize * sizeof(float));
	layout = new VertexBufferLayout();
	layout->Push<float>(3);
	layout->Push<float>(2);
	layout->Push<float>(3);
	va->AddBuffer(*vb, *layout);
	ib = new IndexBuffer(indices, indicesSize);

	translation = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	drawMode = DrawMode::TRIANGLES;
}

Object::~Object()
{
	delete va;
	delete vb;
	delete ib;
}

void Object::Bind(Shader* shader) const
{
	shader->Bind();
	int texturePointer = 0;

	for (int m = 0; m < materials.size(); m++) {
		for (int t = 0; t < materials[m]->textures.size(); t++) {
			materials[m]->textures[t]->Bind(texturePointer);
			shader->SetUniform1i(materials[m]->textures[t]->GetName(), texturePointer);
			texturePointer++;
		}
	}
	for (int i = 0; i < textures.size(); i++) {
		textures[i]->Bind(texturePointer);
		shader->SetUniform1i(textures[i]->GetName(), texturePointer);
		texturePointer++;
	}

	shader->SetUniform3fv("u_lightPositions", lightPositions);
	shader->SetUniform3fv("u_lightColours", lightColours);
	shader->SetUniform3f("u_colour", colour);

	va->Bind();
	ib->Bind();
}

void Object::Unbind() const
{

	va->Unbind();
	ib->Unbind();
}

void Object::AddTexture(Texture* texture)
{
	textures.push_back(texture);
}

void Object::AddMaterial(Material* material)
{
	materials.push_back(material);
}

void Object::AddLight()
{
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 colour = glm::vec3(200, 200, 200);
	lightPositions.push_back(position);
	lightColours.push_back(colour);
}
