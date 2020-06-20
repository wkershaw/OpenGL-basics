#pragma once
#include <vector>
#include "Texture.h"

class Material {
public:
	Material();
	~Material();

	void AddTexture(Texture* texture) { textures.push_back(texture); };

	std::vector<Texture*> textures;
private:
};
