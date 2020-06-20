#pragma once
#include "OPENGL.h"
#include <string>

class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
	std::string m_name;

public:
	Texture(const std::string& name, const std::string& filepath);
	~Texture();

	void Bind(unsigned int slot = 0);
	void UnBind();

	inline int GetWidth() const { return m_Width; };
	inline int GetHeight() const { return m_Height; };
	inline std::string GetName() const { return m_name; };
};