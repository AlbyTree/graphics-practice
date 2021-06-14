#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

// Check a condition: if it's false, halt execution 
// and set a breakpoint where ASSERT() was called
#define ASSERT(x) if (!(x)) __debugbreak();
#ifdef _DEBUG
	// Clear all the previous flags,
	// call the x function and check for errors
	#define GLCall(x) GLClearError();\
		x;\
		ASSERT(GLLogCall(#x, __FILE__, __LINE__))
#else
    // Just call the OpenGL function
	#define GLCall(x) x
#endif

// Clear all the OpenGL context errors flags.
void GLClearError();
// Tries to execute an OpenGL function: if the function execution produced errors,
// it prints the errors codes, the file and the line in the file of the instruction that caused the error.
// Returns false is errors occured, true otherwise.
bool GLLogCall(const char* function, const char* file, int line);

// Describes a simple renderer: draws triangles vertices using the order dictated by the indices in the index buffer
// with the vertices properties stored into a vertex array and a shader program.
// It doesn't take care of uniforms so they must be set outside the renderer draw call.
class Renderer
{
private:

public:
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};
