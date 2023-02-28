#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout;
enum VABufferState { NO_BUFFER = 0, UNINIT_BUFFER = 1, INIT_BUFFER = 2 };

// Describes a vertex array that stores a single vertex buffer.
class VertexArray
{
private:
	unsigned int m_RendererID;
	VABufferState m_BufferState;
public:
	VertexArray();
	~VertexArray();

	void Bind() const;
	void Unbind();
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout, bool isBufferInit);
	// Replace buffer data starting from the beginning of the buffer.
	void ReplaceBufferData(const void* data, unsigned int size);
	VABufferState GetBufferState() const;
};
