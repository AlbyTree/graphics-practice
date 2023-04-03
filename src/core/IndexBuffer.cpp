#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    : m_Count(count)
{
    // If by chance, OpenGL unsigned int is not the usual 4 bytes...halt the execution.
    ASSERT(sizeof(unsigned int) == sizeof(GLuint))

    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
	m_Init = true;
	Unbind();
}

IndexBuffer::IndexBuffer()
{
    // If by chance, OpenGL unsigned int is not the usual 4 bytes...halt the execution.
    ASSERT(sizeof(unsigned int) == sizeof(GLuint))

    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	m_Count = 0;
	m_Init = false;
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::ReplaceBufferData(const void* data, unsigned int count)
{
	Bind();
	if (!m_Init)
	{
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW))
		m_Init = true;
	}
	else
	{
		GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count, data))
	}
	m_Count = count;
	Unbind();
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
