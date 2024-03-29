#pragma once

#include <vector>

#include "GL/glew.h"

#include "Renderer.h"

// USE ONLY INTERNALLY //
namespace HelperCondition
{
	template <typename T>
	constexpr bool alwaysFalse = false;
}

// Describes a single attribute of a vertex buffer.
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


// Describes the layout of a buffer that stores vertices informations as a vector of elements (attributes).
class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout()
		: m_Stride(0) {}

	template <typename T>
	void AddAttrib(unsigned int count)
	{
		// static_assert is checked prior template instantiation:
		// a workaround is to make the condition dependent on the type
		// so the compiler needs to wait until the instantiation is performed.
		static_assert(HelperCondition::alwaysFalse<T>);
	}

	template <>
	void AddAttrib<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template <>
	void AddAttrib<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template <>
	void AddAttrib<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }
};
