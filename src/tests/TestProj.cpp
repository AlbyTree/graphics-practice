#include "TestProj.h"
#include "../core/Renderer.h"

#include "../deps/imgui/imgui.h"
#include "../deps/glm/glm.hpp"
#include "../deps/glm/gtc/matrix_transform.hpp"

namespace test
{
	TestPerspProjSteps::TestPerspProjSteps()
        : m_point_e(glm::vec3(10.0f, 10.0f, -5.0f)), 
			m_l(-50.0f), m_r(50.0f), 
			m_t(25.0f), m_b(-25.0f), 
			m_n(-2.0f), m_f(-10.0f),
			m_point_p(glm::vec3()), m_point_c(glm::vec4()), m_point_NDC(glm::vec3())
	{
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

	}

	void TestPerspProjSteps::OnImGuiRenderer()
	{
	}

    void TestPerspProjSteps::ProjectPointToNearPlane()
    {
        m_point_p.x = (m_n * m_point_e.x) / (-m_point_e.z);
        m_point_p.y = (m_n * m_point_e.y) / (-m_point_e.z);
        m_point_p.z = m_n;
    }

    void TestPerspProjSteps::MapPointToClipSpace()
    {
        m_point_c.x = ((2 * m_point_p.x) / (m_r - m_l)) - ((m_r + m_l) / (m_r - m_l));
        m_point_c.y = ((2 * m_point_p.y) / (m_t - m_b)) - ((m_t + m_b) / (m_t - m_b));
        m_point_c.z = ((((m_f+m_n)/(m_f-m_n))*(-m_point_e.z))-((2*m_f*m_n)/(m_f-m_n)))/(-m_point_e.z);
        m_point_c.w = m_point_e.z;
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
