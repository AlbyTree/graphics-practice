#include "TestMesh.h"

#include "core/Renderer.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "imgui/imgui.h"

test::TestMesh::TestMesh()
{
	m_Mesh.AddPositions({{-1,0,0}, {0,1,0}, {1,0,0}});
	m_Mesh.AddColor({1, 0.5f, 0.2f, 1.f});
	m_Mesh.AddColor({});
	m_Mesh.AddColor({});
	m_Mesh.AddIndices({0,2,1});

	m_Mesh.UpdateBuffers();

    m_Shader = std::make_unique<Shader>("res/shaders/Default.shader");
}

void test::TestMesh::OnRenderer()
{
    Renderer r;
    
    glm::mat4 model, view, projection;
    glm::vec3 modelScale(100);
    model = glm::scale(glm::mat4(1.f), modelScale);
    view = glm::mat4(1.f);
    projection = glm::ortho(-480.0f, 480.0f, -270.0f, 270.0f);

    m_Shader->Bind();
    
    m_Shader->SetUniformMat4f("u_Model", model);
    m_Shader->SetUniformMat4f("u_View", view);
    m_Shader->SetUniformMat4f("u_Projection", projection);

    r.Draw(m_Mesh.GetVAO(), m_Mesh.GetIB(), *m_Shader);
    
    m_Shader->Unbind();
}

void test::TestMesh::OnImGuiRenderer()
{
	if (ImGui::Button("Change Color"))
	{
		m_Mesh.SetVerticesColors({1}, {{0.2f, 0.2f, 0.2f, 1.f}});
		m_Mesh.SetVertexPosition(1, {0,0.5,0});
		m_Mesh.UpdateBuffers();
	}
}
