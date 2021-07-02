#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
        GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::Bind() const
{
        GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
        GLCall(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		// Specifichiamo come costruire l'array di indici e come sono fatti gli attributi di un vertex.
		// 1 param: numero(indice) dell'attributo
		// 2 param: quanti elementi descrivono l'attributo
		// 3 param: se normalizzare i valori
		// 4 param: quanti byte devi muoverti per raggiungere il prossimo vertice
		// 5 param: quanti byte devi muoverti per raggiungere il prossimo attributo di un vertice
		GLCall(glVertexAttribPointer(i, element.count, element.type,
			element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::ReplaceBufferData(const void* data, unsigned int size) const
{
	GLCall(glBindVertexArray(m_RendererID));
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
}
