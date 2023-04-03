#pragma once

class VertexBuffer
{
private:
	unsigned int m_RendererID;
	
public:
	// Creates a buffer on the GPU and immediatly transfer data into it.
	// `size` is the amount of bytes to send.
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer();
	
	// Creates a buffer on the GPU with `size` bytes left uninitialized.
	VertexBuffer(unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};
