#pragma once

#include <string>

#include "Renderer.h"

// Describes a 2D texture stored as a RGBA image.
// The texture is immediatly sent to the GPU after the object creation.
class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height;
	// Bytes per pixel (for RGB we would use 3 bytes, 1 for Red, 1 for Green and 1 for Blue).
	int m_BPP;
public:
	Texture(const std::string& path);
	~Texture();

	// The optional slot parameter tells what texture unit the texture will be bind to.
	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};
