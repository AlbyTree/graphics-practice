#pragma once

class IndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count;
	bool m_Init;
public:
	// Creates an index buffer on the GPU and immediatly transfer data into it.
	IndexBuffer(const unsigned int* data, unsigned int count);
	IndexBuffer();
	void ReplaceBufferData(const void* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }
};
