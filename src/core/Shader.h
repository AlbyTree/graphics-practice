#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

// Contains the vertex and fragment source code of a shader program as two strings.
struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

// Describes a simple shader: it allows to read a single shader file with vertex and fragment shader source codes in GLSL,
// it compiles the shader and creates a shader while saving its ID.
// You can also set various uniforms.
// A cache system CPU side is used when trying to retrieve an uniform location multiple times.
class Shader
{
private:
	std::string m_FilePath;
	// Unique ID of the buffer, given by a specific renderer: there might be multiple renderers that work differently
	// and this ID is going to be understood only by that specific renderer.
	unsigned int m_RendererID;
	// Caching uniforms into an hash map
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	// Creates a full shader program and stores it into the VRAM.
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Set uniforms

	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float f2, float f3);
	void SetUniformMat4f(const std::string& name, glm::mat4& matrix);


private:
	// Parse a single shader source file containing the vertex and fragment shader source codes
	// separated by a comment "#shader vertex" and "#shader fragment".
	ShaderProgramSource ParseShader(const std::string& filepath);
	// Given the type of shader and a source code, it compiles it into the shader type we want.
	// The string must be non empty during this function call or undefined behaviour might occurr.
	unsigned int CompileShader(unsigned int type, const std::string& source);
	// Creates a shader program by taking two shaders source code.
	// It returns an ID that can be used in an graphics API context.
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	// Returns the uniform location specified by name: if the uniform name was saved in the object internal cache,
	// then it's immediatly returned; otherwise, an OpenGL call is going to be executed for retreving the location.
	// The uniform is saved in the cache either OpenGL returns an "error" for that uniform.
	int GetUniformLocation(const std::string& name);
};
