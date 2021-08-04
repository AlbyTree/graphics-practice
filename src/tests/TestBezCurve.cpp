#include "TestBezCurve.h"
#include "../core/Renderer.h"

#include "../deps/imgui/imgui.h"
#include "../deps/glm/glm.hpp"
#include "../deps/glm/gtc/matrix_transform.hpp"
#include "../deps/glm/gtc/type_ptr.hpp"

namespace test
{
	TestBezCurve::TestBezCurve()
		: m_NumSegments(100),
		m_P1(glm::vec3(-0.5f, -0.5f, 0.0f)),
		m_P2(glm::vec3(-0.2, 0.2f, 0.0f)),
		m_P3(glm::vec3(0.12, 0.12, 0.0f)),
		m_P4(glm::vec3(0.5f, -0.5f, 0.0f)),
		m_resize_factor(200), m_translate(0.0f),
		m_ShowCurvePoints(false), m_ControlPoints(false)
	{
		m_CurvePoints = computePointOnCurveBez(m_NumSegments, m_P1, m_P2, m_P3, m_P4);
		// The number of indices depends on the number of points generated for the curve.
		std::vector<unsigned int> indicesCurvePoints;
		for (int i = 0; i < m_CurvePoints.size(); i++)
			indicesCurvePoints.push_back(i);

		// Curve points
        m_VAO_CurvePoints = std::make_unique<VertexArray>();
        m_VertexBuffer_CurvePoints = std::make_unique<VertexBuffer>(
			m_CurvePoints.data(), 3*m_CurvePoints.size()*sizeof(float));

        VertexBufferLayout vbLayout;
        vbLayout.AddAttrib<float>(3);

        m_VAO_CurvePoints->AddBuffer(*m_VertexBuffer_CurvePoints, vbLayout, true);
        m_IndexBuffer_CurvePoints = std::make_unique<IndexBuffer>(
			indicesCurvePoints.data(), indicesCurvePoints.size());

		// Control points
		m_ControlPoints.push_back(m_P1);
		m_ControlPoints.push_back(m_P2);
		m_ControlPoints.push_back(m_P3);
		m_ControlPoints.push_back(m_P4);
		unsigned int indicesControlPoints[4] = { 0,1,2,3 };
		
        m_VAO_ControlPoints = std::make_unique<VertexArray>();
        m_VertexBuffer_ControlPoints = std::make_unique<VertexBuffer>(
			m_ControlPoints.data(), 3*m_ControlPoints.size()*sizeof(float));

        m_VAO_ControlPoints->AddBuffer(*m_VertexBuffer_ControlPoints, vbLayout, true);
        m_IndexBuffer_ControlPoints = std::make_unique<IndexBuffer>(indicesControlPoints, 4);

        m_Shader = std::make_unique<Shader>("res/shaders/BezCurve.shader");
	}

	TestBezCurve::~TestBezCurve()
	{
	}

	void TestBezCurve::OnUpdate(float deltaTime)
	{
	}

	void TestBezCurve::OnRenderer()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
        GLCall(glEnable(GL_PROGRAM_POINT_SIZE));

        Renderer renderer;
		m_Shader->Bind();
		
		// Define MVP matrix
		glm::mat4 Projection, Model, MVP = glm::mat4(1.0f);
		Projection = glm::ortho(-480.0f, 480.0f, -270.0f, 270.0f);
		glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(m_resize_factor, m_resize_factor, 0.0f));
		glm::mat4 Translate = glm::translate(glm::mat4(1.0f), m_translate);
		Model = Translate * Scale;
		MVP = Projection * Model;

		// Save vertices info application side
		m_UpdatedPointsOnCurve = std::vector<glm::vec3>();
		m_UpdatedControlPoints = std::vector<glm::vec3>();
		for (int i = 0; i < m_CurvePoints.size(); i++)
			m_UpdatedPointsOnCurve.push_back(MVP * glm::vec4(m_CurvePoints[i], 1.0f));
		for (int i = 0; i < m_ControlPoints.size(); i++)
			m_UpdatedControlPoints.push_back(MVP * glm::vec4(m_ControlPoints[i], 1.0f));

		m_Shader->SetUniformMat4f("u_MVP", MVP);

		renderer.Draw(*m_VAO_CurvePoints, *m_IndexBuffer_CurvePoints, *m_Shader, LINE_STRIP);
		if (m_ShowCurvePoints)
			renderer.Draw(*m_VAO_CurvePoints, *m_IndexBuffer_CurvePoints, *m_Shader, POINTS);
		if (m_ShowControlPoints)
			renderer.Draw(*m_VAO_ControlPoints, *m_IndexBuffer_ControlPoints, *m_Shader, POINTS);
	}

	void TestBezCurve::OnImGuiRenderer()
	{
		ImGui::SliderFloat("Resize", &m_resize_factor, -2000.0f, 2000.0f);
		ImGui::SliderFloat("Move X", &m_translate.x, -1000.0f, 1000.0f);
		ImGui::SliderFloat("Move Y", &m_translate.y, -1000.0f, 1000.0f);

		if (ImGui::Button("Show Control Points"))
			m_ShowControlPoints = m_ShowControlPoints ? false : true;
		if (m_ShowControlPoints)
		{
			for (int i = 0; i < m_UpdatedControlPoints.size(); i++)
			{
				ImGui::Text("ContrP%d: %f,%f", i, 
					m_UpdatedControlPoints[i].x, m_UpdatedControlPoints[i].y);
			}
		}
		
		if (ImGui::Button("Show Curve Points"))
			m_ShowCurvePoints = m_ShowCurvePoints ? false : true;
		if (m_ShowCurvePoints)
		{
			for (int i = 0; i < m_UpdatedPointsOnCurve.size(); i++)
			{
				ImGui::Text("V%d: %f,%f", i, 
					m_UpdatedPointsOnCurve[i].x, m_UpdatedPointsOnCurve[i].y);
			}
		}
	}
	
	std::vector<glm::vec3> TestBezCurve::computePointOnCurveBez(int numSegments, glm::vec3 P1, glm::vec3 P2, glm::vec3 P3, glm::vec3 P4)
	{
		std::vector<glm::vec3> curvePoints;
		for (int i = 0; i <= numSegments; ++i) 
		{
			float t = i / (float)numSegments;

			// Compute coefficients
			float k1 = (1 - t) * (1 - t) * (1 - t);
			float k2 = 3 * (1 - t) * (1 - t) * t;
			float k3 = 3 * (1 - t) * t * t;
			float k4 = t * t * t;

			// Weight the four control points using coefficients
			curvePoints.push_back((P1 * k1 + P2 * k2 + P3 * k3 + P4 * k4));
		}

		return curvePoints;
	}
}
