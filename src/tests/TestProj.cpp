#include "TestProj.h"
#include "../core/Renderer.h"

#include "../deps/imgui/imgui.h"
#include "../deps/glm/glm.hpp"
#include "../deps/glm/gtc/matrix_transform.hpp"
#include "../deps/glm/gtc/type_ptr.hpp"

namespace test
{
	TestPerspProjSteps::TestPerspProjSteps()
        : m_point_e(glm::vec3(0.0f, 0.0f, -2.0f)), 
			m_l(-470.0f), m_r(470.0f), 
			m_t(270.0f), m_b(-270.0f), 
			m_n(-1.0f), m_f(-5.0f),
			m_point_p(glm::vec3()), m_point_c(glm::vec4()), m_point_NDC(glm::vec3())
	{
        unsigned int indices[] = {0};

        GLCall(glEnable(GL_PROGRAM_POINT_SIZE));

        m_VAO = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(glm::value_ptr(m_point_NDC), sizeof(float));

        VertexBufferLayout vbLayout;
        vbLayout.Push<float>(3);

        m_VAO->AddBuffer(*m_VertexBuffer, vbLayout);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 1);

        m_Shader = std::make_unique<Shader>("res/shaders/PerspProjSteps.shader");
	}

	TestPerspProjSteps::~TestPerspProjSteps()
	{
	}

	void TestPerspProjSteps::OnUpdate(float deltaTime)
	{
	}

	void TestPerspProjSteps::OnRenderer()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        ProjectPointToNearPlane();
        MapPointToClipSpace();
        NDCTransf();

		m_Shader->Bind();
		m_Shader->SetUniform3fv("u_NDCPos", 3, glm::value_ptr(m_point_NDC));

		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader, POINTS);
	}

	void TestPerspProjSteps::OnImGuiRenderer()
	{
        ImGui::Text("Eye Space Point: ");
        ImGui::SliderFloat3("", &m_point_e.x, -1000.0f, 1000.0f);
        ImGui::Text("PROJ PLANES PARAMS: \nl = %f r = %f \nt = %f b = %f \nn = %f f = %f",
            m_l, m_r, m_t, m_b, m_n, m_f);
        ImGui::Text("Projected Point: [%f,%f,%f]", m_point_p.x, m_point_p.y, m_point_p.z);
        ImGui::Text("Clipped Point: [%f,%f,%f,%f]", m_point_c.x, m_point_c.y, m_point_c.z, m_point_c.w);
        ImGui::Text("NDC Point: [%f,%f,%f]", m_point_NDC.x, m_point_NDC.y, m_point_NDC.z);
	}

    void TestPerspProjSteps::ProjectPointToNearPlane()
    {
        m_point_p.x = (m_n * m_point_e.x) / (-m_point_e.z);
        m_point_p.y = (m_n * m_point_e.y) / (-m_point_e.z);
        m_point_p.z = m_n;
    }

    void TestPerspProjSteps::MapPointToClipSpace()
    {
        m_point_c.x = ((2 * m_n * m_point_e.x) / (m_r - m_l)) + (((m_r + m_l) / (m_r - m_l))*m_point_e.z);
        m_point_c.y = ((2 * m_n * m_point_e.y) / (m_t - m_b)) + (((m_t + m_b) / (m_t - m_b))*m_point_e.z);
        m_point_c.z = ((((m_f + m_n) / (m_f - m_n)) * (-m_point_e.z)) - ((2 * m_f * m_n) / (m_f - m_n)));
        m_point_c.w = -m_point_e.z;
    }

    void TestPerspProjSteps::NDCTransf()
    {
        m_point_NDC.x = m_point_c.x / m_point_c.w;
        m_point_NDC.y = m_point_c.y / m_point_c.w;
        m_point_NDC.z = m_point_c.z / m_point_c.w;
    }

    void TestPerspProjSteps::ProjectPoint()
    {
        ProjectPointToNearPlane();
        MapPointToClipSpace();
        NDCTransf();
    }
}
