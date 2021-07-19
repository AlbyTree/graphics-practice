#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout;
enum VABufferState { NO_BUFFER = 0, UNINIT_BUFFER = 1, INIT_BUFFER = 2 };

// Describes a single vertex array that stores a single vertex buffer with multiple vertex buffer layouts.
// You cannot remove a buffer layout once it's stored into a specific vertex array object: you can only unbind 
// the entire vertex array (which will cause to lose informations about the previously binded vertex buffer layouts).
// Only the ID that a renderer gives to the vertex array is saved inside the class object.
// The constructor just request an ID from the renderer.
class VertexArray
{
private:
	// Unique ID of the buffer, given by a specific renderer: there might be multiple renderers that work differently
	// and this ID is going to be understood only by that specific renderer.
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
