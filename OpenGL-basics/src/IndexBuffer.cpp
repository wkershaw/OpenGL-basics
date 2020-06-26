#include "IndexBuffer.h"

IndexBuffer::IndexBuffer() {
    m_count = 0;
    m_RendererID = 0;
}
IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
{
    m_count = count;
    GLCALL(glGenBuffers(1, &m_RendererID)); //Generate a buffer and return its ID
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID)); //Set the current buffer in the OpenGL state machine
    GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_STATIC_DRAW)); //Add position data to the buffer

}

IndexBuffer::~IndexBuffer()
{
    GLCALL(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
    GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
