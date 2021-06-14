#pragma once

#include <vector>

#include <GL/glew.h>

#include "Renderer.h"

// Describes a single attribute of a vertex buffer.
// It saves the type of the values used to represent the attribute, the number of values used for the attribute
// and if those values have to be normalized.
struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	// Only GL_FLOAT, GL_UNSIGNED_INT and GL_UNSIGNED_BYTE are supported:
	// any other type halts the application execution with a debug breakpoint.
	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
		case GL_UNSIGNED_BYTE:	return 1;
		}
		ASSERT(false)
		return 0;
	}
};


// Describes the layout of a buffer that stores vertices informations as a vector of elements (attributes)
// where each element has its own "layout" i.e. how many values it has, what type they are and if they have to be normalized.
// It also stores the stride of the entire vertex i.e. how many bytes have to be jumped to reach the next vertex position in the buffer.
class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout()
		: m_Stride(0) {}

	template <typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}

	template <>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template <>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template <>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }
};
