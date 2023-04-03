#include "Mesh.h"
#include <iostream>

Mesh::Mesh()
    : m_VB(nullptr), m_VAO(nullptr), m_IB(nullptr)
{
    m_VBLayout.AddAttrib<float>(3);
    m_VBLayout.AddAttrib<float>(4);

    m_VAO.reset(new VertexArray());
    m_VB.reset(new VertexBuffer());
    m_VAO->AddBuffer(*m_VB, m_VBLayout, false);
	
    m_IB.reset(new IndexBuffer());
}

void Mesh::SetVertexPosition(unsigned int i, const glm::vec3& p)
{
	if (i < m_Positions.size())
		m_Positions[i] = p;
}

void Mesh::SetVertexColor(unsigned int i, const glm::vec4& c)
{
	if (i < m_Colors.size())
		m_Colors[i] = c;
}

void Mesh::SetIndex(unsigned int i, unsigned index)
{
	if (i < m_Indices.size())
		m_Indices[i] = index;
}

void Mesh::SetVerticesPositions(const std::vector<unsigned int>& range, const std::vector<glm::vec3>& p)
{
	if (range.size() == p.size())
	{
		if (range.front() < m_Positions.size() && range.back() < m_Positions.size())
		{
			unsigned int j = 0;
			for (const auto& i : range)
			{
				m_Positions[i] = p[j];
				j++;
			}
		}
	}
}

void Mesh::SetVerticesColors(const std::vector<unsigned>& range, const std::vector<glm::vec4>& c)
{
	if (range.size() == c.size())
	{
		if (range.front() < m_Colors.size() && range.back() < m_Colors.size())
		{
			unsigned int j = 0;
			for (const auto& i : range)
			{
				m_Colors[i] = c[j];
				j++;
			}
		}
	}
}

void Mesh::SetIndices(const std::vector<unsigned>& range, const std::vector<unsigned>& indices)
{
	if (range.size() == indices.size())
	{
		if (range.front() < m_Indices.size() && range.back() < m_Indices.size())
		{
			unsigned int j = 0;
			for (const auto& i : range)
			{
				m_Indices[i] = indices[j];
				j++;
			}
		}
	}
}

std::vector<MeshData> Mesh::GetVertices() const
{
	std::vector<MeshData> temp;
	for (unsigned int i = 0; i < m_Positions.size(); i++)
		temp.emplace_back(m_Positions[i], m_Colors[i]);
	
	return temp;
}

void Mesh::UpdateBuffers()
{
	if (!m_IsValid())
	{
		std::cout << "Warning: some mesh's attributes have less data than others." << "\n";
		return;
	}
	
	std::vector<MeshData> temp;
	for (unsigned int i = 0; i < m_Positions.size(); i++)
		temp.emplace_back(m_Positions[i], m_Colors[i]);
	
	m_VAO->ReplaceBufferData(temp.data(), temp.size() * sizeof(MeshData));
	
	m_Indices.resize(m_Indices.size());
	m_IB->ReplaceBufferData(m_Indices.data(), m_Indices.size());
}

