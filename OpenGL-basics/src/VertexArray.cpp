#include "VertexArray.h"
#include "OPENGL.h"

VertexArray::VertexArray()
{
	GLCALL(glGenVertexArrays(1, &m_RendererID));
	GLCALL(glBindVertexArray(m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCALL(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (int i = 0; i < elements.size() ; i++)
	{
		const auto& element = elements[i];
		GLCALL(glEnableVertexAttribArray(i)); //Enable an attribute for the buffer, defined on the next line
		GLCALL(glVertexAttribPointer(i, element.count, element.type, element.normalised, layout.GetStride(), (const void*)offset)); //Declare the layout of the buffer data
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	GLCALL(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCALL(glBindVertexArray(0));
}
