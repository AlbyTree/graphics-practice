#pragma once

// Class that represents an array buffer, stored on the GPU side, which is used to store vertices data.
// Only the ID that a renderer gives to the buffer is saved inside the class object.
// The constructor allocates a buffer on the VRAM and immediately transfers the data into the buffer.
class VertexBuffer
{
private:
	// Unique ID of the buffer, given by a specific renderer: there might be multiple renderers that work differently
	// and this ID is going to be understood only by that specific renderer.
	unsigned int m_RendererID;
public:
	// Creates a buffer on the GPU and immediatly transfer data into it.
	// size is the amount of bytes that data will occupy in VRAM.
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	// Bind the buffer to GL_ARRAY_BUFFER .
	void Bind() const;
	// Unbind the buffer from GL_ARRAY_BUFFER .
	void Unbind() const;
};
