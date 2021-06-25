#include "Renderer.h"

#include <iostream>

void GLClearError()
{
    // Just clear the flags, we don't care to store them
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    // Keep retrieving flags until you run out
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error (" << error << ")" 
            << "(hex: 0x" << std::hex << error 
            << std::dec << ")] AT " 
            << function << std::endl << "IN FILE: " << file 
            << std::endl << "LINE:" << line << std::endl;
        return false;
    }
    return true;
}

void Renderer::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, Primitive prim) const
{
    shader.Bind();

    va.Bind();
    ib.Bind();

	// It draws what's bound to GL_ELEMENT_ARRAY_BUFFER
	// otherwise I have to specify a pointer to an indices buffer
	GLCall(glDrawElements(prim, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}
