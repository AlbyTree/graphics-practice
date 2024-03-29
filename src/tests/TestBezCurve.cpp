#include "TestBezCurve.h"
#include "core/Renderer.h"
#include "core/Utils.h"

#include "deps/imgui/imgui.h"
#include "deps/glm/glm.hpp"
#include "deps/glm/gtc/matrix_transform.hpp"
#include "deps/glm/gtc/type_ptr.hpp"

namespace test
{
	TestBezCurve::TestBezCurve()
		: m_NumCurvePoints(100), m_CurvePoints(m_NumCurvePoints),
		m_resize_factor(200), m_translate(0.0f), m_curve_width(2.0f), 
		m_ShowCurvePoints(false), m_ControlPoints(false)
	{
		m_ControlPoints.push_back(glm::vec3(-0.5f, -0.5f, 0.0f));
		m_ControlPoints.push_back(glm::vec3(-0.2, 0.2f, 0.0f));
		m_ControlPoints.push_back(glm::vec3(0.12, 0.12, 0.0f));
		m_ControlPoints.push_back(glm::vec3(0.5f, -0.5f, 0.0f));
		m_ControlPointsCopy = m_ControlPoints;

		compgraphutils::generateCurvePointsBez(m_NumCurvePoints, m_ControlPoints.data(), m_CurvePoints);
		// The number of indices depends on the number of points generated for the curve
		std::vector<unsigned int> indicesCurvePoints;
		for (int i = 0; i < m_CurvePoints.size(); i++)
			indicesCurvePoints.push_back(i);

		// Curve points buffers
        m_VAO_CurvePoints = std::make_unique<VertexArray>();
		m_VertexBuffer_CurvePoints = std::make_unique<VertexBuffer>(
			m_CurvePoints.data(), 3*m_CurvePoints.size()*sizeof(float));

        VertexBufferLayout vbLayout;
        vbLayout.AddAttrib<float>(3);

        m_VAO_CurvePoints->AddBuffer(*m_VertexBuffer_CurvePoints, vbLayout, true);

        m_IndexBuffer_CurvePoints = std::make_unique<IndexBuffer>(
			indicesCurvePoints.data(), indicesCurvePoints.size());

		// Control points buffers
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
		// Anti-aliasing for line primitive
        GLCall(glEnable(GL_LINE_SMOOTH));
        GLCall(glLineWidth(m_curve_width));

        Renderer renderer;
		m_Shader->Bind();
		
		// Define MVP matrix
		glm::mat4 Projection, Model, MVP = glm::mat4(1.0f);
		Projection = glm::ortho(-480.0f, 480.0f, -270.0f, 270.0f);
		glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(m_resize_factor, m_resize_factor, 0.0f));
		glm::mat4 Translate = glm::translate(glm::mat4(1.0f), m_translate);
		Model = Translate * Scale;
		MVP = Projection * Model;

		// If I change one of the control points, re-compute the curve points
		for (int i = 0; i < m_ControlPoints.size(); i++)
		{
			if (m_ControlPoints[i] != m_ControlPointsCopy[i])
			{
					UpdateCurveControlPoints();
					break;
			}
		}

		// If I change the number of curve points, compute the new curve points
		if (m_NumCurvePoints != m_CurvePoints.size())
			UpdateCurvePoints();

		// Save info application side
		m_UpdatedPointsOnCurve = std::vector<glm::vec3>();
		m_UpdatedControlPoints = std::vector<glm::vec3>();
		for (int i = 0; i < m_CurvePoints.size(); i++)
			m_UpdatedPointsOnCurve.push_back(Model * glm::vec4(m_CurvePoints[i], 1.0f));
		for (int i = 0; i < m_ControlPoints.size(); i++)
			m_UpdatedControlPoints.push_back(Model * glm::vec4(m_ControlPoints[i], 1.0f));

		m_Shader->SetUniformMat4f("u_MVP", MVP);

		m_Shader->SetUniform3fv("u_Color", 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
		renderer.Draw(*m_VAO_CurvePoints, *m_IndexBuffer_CurvePoints, *m_Shader, LINE_STRIP);
		if (m_ShowCurvePoints)
		{
			m_Shader->SetUniform3fv("u_Color", 1, glm::value_ptr(glm::vec3(0.7, 0.0f, 0.0f)));
			renderer.Draw(*m_VAO_CurvePoints, *m_IndexBuffer_CurvePoints, *m_Shader, POINTS);
		}
		if (m_ShowControlPoints)
		{
			GLCall(glLineWidth(1.0f));
			m_Shader->SetUniform3fv("u_Color", 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 1.0f)));
			renderer.Draw(*m_VAO_ControlPoints, *m_IndexBuffer_ControlPoints, *m_Shader, LINE_STRIP);
			renderer.Draw(*m_VAO_ControlPoints, *m_IndexBuffer_ControlPoints, *m_Shader, POINTS);
		}
	}

	void TestBezCurve::OnImGuiRenderer()
	{
		ImGui::SliderFloat("Resize", &m_resize_factor, -2000.0f, 2000.0f);
		ImGui::SliderFloat("Move X", &m_translate.x, -1000.0f, 1000.0f);
		ImGui::SliderFloat("Move Y", &m_translate.y, -1000.0f, 1000.0f);
		ImGui::InputFloat("Curve width", &m_curve_width, 0.1f);

		ImGui::SliderFloat("Move CP1 X", &m_ControlPoints[0].x, -10.0f, 10.0f);
		ImGui::SliderFloat("Move CP1 Y", &m_ControlPoints[0].y, -10.0f, 10.0f);
		ImGui::SliderFloat("Move CP2 X", &m_ControlPoints[1].x, -10.0f, 10.0f);
		ImGui::SliderFloat("Move CP2 Y", &m_ControlPoints[1].y, -10.0f, 10.0f);
		ImGui::SliderFloat("Move CP3 X", &m_ControlPoints[2].x, -10.0f, 10.0f);
		ImGui::SliderFloat("Move CP3 Y", &m_ControlPoints[2].y, -10.0f, 10.0f);
		ImGui::SliderFloat("Move CP4 X", &m_ControlPoints[3].x, -10.0f, 10.0f);
		ImGui::SliderFloat("Move CP4 Y", &m_ControlPoints[3].y, -10.0f, 10.0f);

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
		
		ImGui::InputInt("Number of curve points", &m_NumCurvePoints);

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

	void TestBezCurve::UpdateCurveControlPoints()
	{
		compgraphutils::generateCurvePointsBez(m_NumCurvePoints, m_ControlPoints.data(), m_CurvePoints);

		// We first need to bind the right vertex buffer before sending the data using the VAO!
		m_VertexBuffer_CurvePoints->Bind();
		m_VAO_CurvePoints->ReplaceBufferData(m_CurvePoints.data(), 3 * m_CurvePoints.size() * sizeof(float));

		// Save control points if they are changed later
		m_ControlPointsCopy = m_ControlPoints;

		m_VertexBuffer_ControlPoints->Bind();
		m_VAO_ControlPoints->ReplaceBufferData(m_ControlPoints.data(), 3 * m_ControlPoints.size() * sizeof(float));
	}

	void TestBezCurve::UpdateCurvePoints()
	{
		m_CurvePoints.resize(m_NumCurvePoints);
		compgraphutils::generateCurvePointsBez(m_NumCurvePoints, m_ControlPoints.data(), m_CurvePoints);

		std::vector<unsigned int> indicesCurvePoints;
		for (int i = 0; i < m_CurvePoints.size(); i++)
			indicesCurvePoints.push_back(i);

		m_VAO_CurvePoints.reset();
        m_VAO_CurvePoints = std::make_unique<VertexArray>();
		m_VertexBuffer_CurvePoints.reset();
        m_VertexBuffer_CurvePoints = std::make_unique<VertexBuffer>(
			m_CurvePoints.data(), 3*m_CurvePoints.size()*sizeof(float));

        VertexBufferLayout vbLayout;
        vbLayout.AddAttrib<float>(3);

        m_VAO_CurvePoints->AddBuffer(*m_VertexBuffer_CurvePoints, vbLayout, true);
		m_IndexBuffer_CurvePoints.reset();
        m_IndexBuffer_CurvePoints = std::make_unique<IndexBuffer>(
			indicesCurvePoints.data(), indicesCurvePoints.size());
	}
}
