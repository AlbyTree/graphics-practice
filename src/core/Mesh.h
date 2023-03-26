#pragma once

#include <vector>
#include <memory>
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"

struct MeshData
{
    MeshData() = default;
    MeshData(const glm::vec3& p, const glm::vec4& c) : position(p), color(c) {}
    MeshData(const glm::vec3& p) : position(p) {}
    MeshData(const glm::vec4& c) : color(c) {}
    
    glm::vec3 position{0.f, 0.f, 0.f};
    glm::vec4 color{1.f, 1.f, 1.f, 1.f};
};

class Mesh
{
public:
    Mesh();
    ~Mesh() = default;

    std::vector<glm::vec3> GetPositions();
    inline unsigned int GetNumVertices() const { return m_Data.size(); };
    void SetVertices(std::vector<MeshData>&& data);
    void SetIndices(std::vector<unsigned int>&& indices);
    void SetVertexColor(unsigned int index, const glm::vec4& c);

    inline const VertexArray& GetVAO() const { return *m_VAO; }
    inline const IndexBuffer& GetIB() const { return *m_IB; }

private:
    std::vector<MeshData> m_Data;
    std::vector<unsigned int> m_Indices;
    VertexBufferLayout m_VBLayout;
    std::unique_ptr<VertexBuffer> m_VB;
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<IndexBuffer> m_IB;
};

