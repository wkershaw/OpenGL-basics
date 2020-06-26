#pragma once
#include "OPENGL.h"

class IndexBuffer {
public:
	IndexBuffer();
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_count; };

private:
	unsigned int m_RendererID;
	unsigned int m_count;

};