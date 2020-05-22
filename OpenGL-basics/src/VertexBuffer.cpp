#include "VertexBuffer.h"
#include "Renderer.h";


VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    GLCALL(glGenBuffers(1, &m_RendererID)); //Generate a buffer and return its ID
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); //Set the current buffer in the OpenGL state machine
    GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW)); //Add position data to the buffer

}

VertexBuffer::~VertexBuffer()
{
    GLCALL(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
