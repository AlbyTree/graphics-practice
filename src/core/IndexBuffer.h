#pragma once

// Class that represents an index buffer, stored on the GPU side, which is used to decide in which order
// a primitives vertices should be choosen.
// Only the ID that a renderer gives to the buffer is saved inside the class object.
// The constructor allocates a buffer on the VRAM and immediately transfers the data into the buffer.
class IndexBuffer
{
private:
	// Unique ID of the buffer, given by a specific renderer: there might be multiple renderers that work differently
	// and this ID is going to be understood only by that specific renderer.
	unsigned int m_RendererID;
	// Number of indices to be used for rendering.
	unsigned int m_Count;
public:
	// Creates an index buffer on the GPU and immediatly transfer data into it.
	// count is the number of indices inside data.
	// Only unsigned int indices can be used.
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	// Bind the buffer to GL_ELEMENT_ARRAY .
	void Bind() const;
	// Unbind the buffer from GL_ELEMENT_ARRAY .
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }
};
