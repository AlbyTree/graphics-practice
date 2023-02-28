#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
        GLCall(glGenVertexArrays(1, &m_RendererID));
		m_BufferState = NO_BUFFER;
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::Bind() const
{
        GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind()
{
        GLCall(glBindVertexArray(0));
		m_BufferState = NO_BUFFER;
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout, bool isBufferInit)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		// 1 param: index of the attribute
		// 2 param: how many elements the attribute has
		// 3 param: if elements need to be normalizd
		// 4 param: how many bytes to jump to reach next vertex
		// 5 param: how many bytes to jump to reach next vertex attribute
		GLCall(glVertexAttribPointer(i, element.count, element.type,
			element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
	m_BufferState = (isBufferInit ? INIT_BUFFER : UNINIT_BUFFER);
}

void VertexArray::ReplaceBufferData(const void* data, unsigned int size)
{
	GLCall(glBindVertexArray(m_RendererID));
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
	m_BufferState = INIT_BUFFER;
}

VABufferState VertexArray::GetBufferState() const
{
	return m_BufferState;
}
