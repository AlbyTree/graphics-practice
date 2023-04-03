#pragma once

#include <iterator>
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

    inline const VertexArray& GetVAO() const { return *m_VAO; }
    inline const IndexBuffer& GetIB() const { return *m_IB; }

	inline void AddPosition(const glm::vec3& p) { m_Positions.emplace_back(p); }
	inline void AddPositions(const std::vector<glm::vec3>& p)
    {
	    std::copy(p.begin(), p.end(), std::back_insert_iterator<std::vector<glm::vec3>>(m_Positions));
    }
	inline void AddColor(const glm::vec4& c) { m_Colors.emplace_back(c); }
	inline void AddColors(const std::vector<glm::vec4>& c)
	{
		std::copy(c.begin(), c.end(), std::back_insert_iterator<std::vector<glm::vec4>>(m_Colors));
	}
	inline void AddIndex(const unsigned int& i) { m_Indices.emplace_back(i); }
	inline void AddIndices(const std::vector<unsigned int>& i)
	{
		std::copy(i.begin(), i.end(), std::back_insert_iterator<std::vector<unsigned int>>(m_Indices));
	}

	void SetVertexPosition(unsigned int i, const glm::vec3& p);
	void SetVertexColor(unsigned int i, const glm::vec4& c);
	void SetIndex(unsigned int i, unsigned int index);
	void SetVerticesPositions(const std::vector<unsigned int>& range, const std::vector<glm::vec3>& p);
	void SetVerticesColors(const std::vector<unsigned int>& range, const std::vector<glm::vec4>& c);
	void SetIndices(const std::vector<unsigned int>& range, const std::vector<unsigned int>& i);

	std::vector<MeshData> GetVertices() const;
	inline const std::vector<glm::vec3>& GetPositions() const { return m_Positions; }
	inline const std::vector<glm::vec4>& GetColors() const { return m_Colors; }
	inline const std::vector<unsigned int>& GetIndices() const { return m_Indices; }
	inline unsigned int GetNumVertices() const { return m_Positions.size(); }

	void UpdateBuffers();

private:
	std::vector<glm::vec3> m_Positions;
	std::vector<glm::vec4> m_Colors;
    std::vector<unsigned int> m_Indices;
	
    VertexBufferLayout m_VBLayout;
    std::unique_ptr<VertexBuffer> m_VB;
    std::unique_ptr<VertexArray> m_VAO;
    std::unique_ptr<IndexBuffer> m_IB;

	bool m_IsValid() const { return (m_Positions.size() == m_Colors.size()); }
};
