#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

// Contains the vertex and fragment source code.
struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

// Describes a simple shader manager (compilation, linking, setting/getting uniforms, ...).
class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	// Creates a full shader program and stores it into the GPU.
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float f2, float f3);
	// Set `count` number of elements of the array `arr`(`count` should be 3).
	void SetUniform3fv(const std::string& name, unsigned int count, const float* arr);
	void SetUniformMat4f(const std::string& name, glm::mat4& matrix);


private:
	// Parse a single shader source file containing the vertex and fragment shader source codes
	// separated by a comment "#shader vertex" and "#shader fragment".
	ShaderProgramSource ParseShader(const std::string& filepath);

	// `type` must be `GL_VERTEX_SHADER`, `GL_FRAGMENT_SHADER` or `GL_GEOMETRY_SHADER`.
	// The string must be non empty or undefined behaviour might occurr.
	unsigned int CompileShader(unsigned int type, const std::string& source);
	
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	// Returns the uniform location specified by name:
	// an internal cache is used to check if the uniform was already saved.
	// The uniform name is saved in the cache regardless of its existence.
	int GetUniformLocation(const std::string& name);
};
