#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	GLCALL(glGenBuffers(1, &m_RendererID)); // tell the gpu , generate a buffer with id of 1 .
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW)); // look at to docs.gl adress to take more info.
}

VertexBuffer::~VertexBuffer()
{
	GLCALL(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));

}

void VertexBuffer::UnBind() const
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

}
