#include "TestProjSteps.h"
#include "../core/Renderer.h"
#include "../core/Utils.h"

#include "../deps/imgui/imgui.h"
#include "../deps/glm/glm.hpp"
#include "../deps/glm/gtc/matrix_transform.hpp"
#include "../deps/glm/gtc/type_ptr.hpp"

namespace test
{
    TestProjSteps::TestProjSteps()
        : m_UpdatedData(),
        m_ProjectedData(), m_ClippedData(), m_NDCData(),
        m_r(470.0f), m_l(-470.0f),
        m_t(270.0f), m_b(-270.0f),
        m_n(1.0f), m_f(5.0f),
        m_fov(glm::radians(45.0f)), m_ar(960.0f / 540.0f),
        m_Projection(), m_TypeOfProj(PERSPECTIVE_PROJ),
        m_Translation(0.0f), m_Resize(1.0f), m_ScalingFactor(1.0f)
	{
        m_Data = compgraphutils::CreateTrianglePositions();
        unsigned int indices[] = {0, 1, 2};

        m_VAO = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(glm::value_ptr(m_Data), sizeof(m_Data));

        VertexBufferLayout vbLayout;
        vbLayout.AddAttrib<float>(3);

        m_VAO->AddBuffer(*m_VertexBuffer, vbLayout, true);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 3);

        m_Shader = std::make_unique<Shader>("res/shaders/ProjSteps.shader");
	}

	TestProjSteps::~TestProjSteps()
	{
	}

	void TestProjSteps::OnUpdate(float deltaTime)
	{
	}

	void TestProjSteps::OnRenderer()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        // Resize and then translate vertices positions
        glm::mat4 UpdatePosition = glm::translate(glm::mat4(1.0f), m_Translation);
        m_Resize = glm::vec2(m_ScalingFactor);
        UpdatePosition = glm::scale(UpdatePosition, glm::vec3(m_Resize, 1.0f));
        for (int i = 0; i < 3; i++)
			m_UpdatedData[i] = UpdatePosition * glm::vec4(m_Data[i], 1.0f);

        // Compute vertices positions projection application side
        for (int i = 0; i < 3; i++)
            m_ProjectedData[i] = m_TypeOfProj ?
            compgraphutils::ProjectPointToNearPlane(m_UpdatedData[i], m_n)
            : compgraphutils::ProjectPointToNearPlane(m_UpdatedData[i], m_n, false);

        for (int i = 0; i < 3; i++)
            m_ClippedData[i] = m_TypeOfProj ?
            compgraphutils::MapPointToClipSpace(m_UpdatedData[i], m_r, m_l, m_t, m_b, m_n, m_f)
            : compgraphutils::MapPointToClipSpace(m_UpdatedData[i], m_r, m_l, m_t, m_b, m_n, m_f, false);

        for (int i = 0; i < 3; i++)
            m_NDCData[i] = compgraphutils::NDCTransf(m_ClippedData[i]);

        // Compute projection matrix for GPU side
        m_Projection = m_TypeOfProj ?
		compgraphutils::CreatePerspectiveMatrix(m_r, m_l, m_t, m_b, m_n, m_f)
		: compgraphutils::CreateOrthoProjMatrix(m_r, m_l, m_t, m_b, m_n, m_f);

        glm::mat4 MVP = m_Projection * UpdatePosition;
        //glm::mat4 MVP = glm::perspective(m_fov, m_ar, m_n, m_f);

		m_Shader->Bind();
        m_Shader->SetUniformMat4f("u_MVP", MVP);

		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
	}

	void TestProjSteps::OnImGuiRenderer()
	{
        ImGui::SliderFloat("Translate X", &m_Translation.x, -20.0f, 20.0f);
        ImGui::SliderFloat("Translate Y", &m_Translation.y, -20.0f, 20.0f);
        ImGui::SliderFloat("Translate Z", &m_Translation.z, -20.0f, 20.0f);
        ImGui::SliderFloat("Resize", &m_ScalingFactor, 0.0f, 100.0f);

        ImGui::Text("PROJ PLANES PARAMS: \n");

        ImGui::PushItemWidth(120.0f);
        ImGui::InputFloat("left,", &m_l, 0.5f);
        ImGui::SameLine();
        ImGui::PushItemWidth(120.0f);
        ImGui::InputFloat("right", &m_r, 0.5f);

        ImGui::PushItemWidth(120.0f);
        ImGui::InputFloat("top,", &m_t, 0.5f);
        ImGui::SameLine();
        ImGui::PushItemWidth(120.0f);
        ImGui::InputFloat("bottom", &m_b, 0.5f);

        //ImGui::PushItemWidth(120.0f);
        //ImGui::InputFloat("fov,", &m_fov, 0.5f);
        //ImGui::SameLine();
        //ImGui::PushItemWidth(120.0f);
        //ImGui::InputFloat("aspect ratio", &m_ar, 0.5f);

        ImGui::PushItemWidth(120.0f);
        ImGui::InputFloat("near,", &m_n, 0.05f);
        ImGui::SameLine();
        ImGui::PushItemWidth(120.0f);
        ImGui::InputFloat("far", &m_f, 0.05f);

        ImGui::Text("VERTICES: \n");
        ImGui::Text("V1: [%f,%f,%f]", m_UpdatedData[0].x, m_UpdatedData[0].y, m_UpdatedData[0].z);
        ImGui::Text("V2: [%f,%f,%f]", m_UpdatedData[1].x, m_UpdatedData[1].y, m_UpdatedData[1].z);
        ImGui::Text("V3: [%f,%f,%f]", m_UpdatedData[2].x, m_UpdatedData[2].y, m_UpdatedData[2].z);

        ImGui::Text("Proj V1: [%f,%f,%f]", m_ProjectedData[0].x, m_ProjectedData[0].y, m_ProjectedData[0].z);
        ImGui::Text("Proj V2: [%f,%f,%f]", m_ProjectedData[1].x, m_ProjectedData[1].y, m_ProjectedData[1].z);
        ImGui::Text("Proj V3: [%f,%f,%f]", m_ProjectedData[2].x, m_ProjectedData[2].y, m_ProjectedData[2].z);

        ImGui::Text("Clip V1: [%f,%f,%f]", m_ClippedData[0].x, m_ClippedData[0].y, m_ClippedData[0].z);
        ImGui::Text("Clip V2: [%f,%f,%f]", m_ClippedData[1].x, m_ClippedData[1].y, m_ClippedData[1].z);
        ImGui::Text("Clip V3: [%f,%f,%f]", m_ClippedData[2].x, m_ClippedData[2].y, m_ClippedData[2].z);

        ImGui::Text("NDC V1: [%f,%f,%f]", m_NDCData[0].x, m_NDCData[0].y, m_NDCData[0].z);
        ImGui::Text("NDC V2: [%f,%f,%f]", m_NDCData[1].x, m_NDCData[1].y, m_NDCData[1].z);
        ImGui::Text("NDC V3: [%f,%f,%f]", m_NDCData[2].x, m_NDCData[2].y, m_NDCData[2].z);

		if (ImGui::Button("Perspective Proj"))
		{
            m_TypeOfProj = PERSPECTIVE_PROJ;
		}
		if (ImGui::Button("Orthographic Proj"))
		{
            m_TypeOfProj = ORTHO_PROJ;
		}
	}
}
