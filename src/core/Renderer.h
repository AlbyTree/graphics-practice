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

enum Primitive { TRIANGLES = GL_TRIANGLES, POINTS = GL_POINTS, LINE_STRIP = GL_LINE_STRIP, LINES = GL_LINES };

// Clear all the OpenGL context errors flags.
void GLClearError();

// Tries to execute an OpenGL function: if the function execution produced errors,
// it prints the errors codes, the file and the line in the file of the instruction that caused the error.
// Returns false is errors occured, true otherwise.
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
private:

public:
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, Primitive prim = TRIANGLES) const;
};
