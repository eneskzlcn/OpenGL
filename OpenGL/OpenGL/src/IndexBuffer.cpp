#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int * data, unsigned int count)
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));//some times this condition not provided by plaform. So check it.

	GLCALL(glGenBuffers(1, &m_RendererID)); // tell the gpu , generate a buffer with id of 1 .
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * count, data, GL_STATIC_DRAW)); // look at to docs.gl adress to take more info.
}

IndexBuffer::~IndexBuffer()
{
	GLCALL(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));

}

void IndexBuffer::UnBind() const
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

}
