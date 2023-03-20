#include "Mesh.h"

Mesh::Mesh()
    : m_Data(0), m_VB(nullptr), m_VAO(nullptr), m_IB(nullptr)
{

}

void Mesh::SetVertices(std::vector<MeshData>&& data)
{
    m_Data = data;
    
    m_VBLayout.AddAttrib<float>(3);
    m_VBLayout.AddAttrib<float>(4);

    m_VAO.reset(new VertexArray());
    m_VB.reset(new VertexBuffer(m_Data.data(), m_Data.size() * sizeof(MeshData)));
    m_VAO->AddBuffer(*m_VB, m_VBLayout, true);
}

void Mesh::SetIndices(std::vector<unsigned>&& indices)
{
    m_Indices = indices;
    m_IB.reset(new IndexBuffer(m_Indices.data(), m_Indices.size()));
}

