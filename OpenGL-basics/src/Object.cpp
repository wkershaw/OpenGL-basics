#include "Object.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

Object::Object(float positions[], int positionsSize, unsigned int indices[], int indicesSize) {
	textures = *new std::vector<Texture*>();
	va = new VertexArray();
	vb = new VertexBuffer(positions, positionsSize * sizeof(float));
	layout = new VertexBufferLayout();
	layout->Push<float>(3);
	va->AddBuffer(*vb, *layout);
	ib = new IndexBuffer(indices, indicesSize);

	translation = glm::vec3(0.0f, 0.0f, -100.0f);

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
	for (int i = 0; i < textures.size(); i++){
		textures[i]->Bind(i);
		shader->SetUniform1i("u_texture"+i, i);
	}
	shader->SetUniform4f("u_colour", colour.r, colour.g, colour.b, colour.a);
	va->Bind();
	ib->Bind();
}

void Object::AddTexture(Texture* texture)
{
	textures.push_back(texture);
}
