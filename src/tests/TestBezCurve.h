#pragma once

#include <memory>

#include "Test.h"

#include "../core/VertexBuffer.h"
#include "../core/VertexBufferLayout.h"

namespace test
{
	class TestBezCurve : public Test
	{
	public:
		TestBezCurve();
		~TestBezCurve();

		void OnUpdate(float deltaTime) override;
		void OnRenderer() override;
		void OnImGuiRenderer() override;

		std::vector<glm::vec3> computePointOnCurveBez(int numSegments, glm::vec3 P1, glm::vec3 P2, glm::vec3 P3, glm::vec3 P4);
		
	private:

		int m_NumSegments;
		// Bezier curve control points
		glm::vec3 m_P1, m_P2, m_P3, m_P4;
		std::vector<glm::vec3> m_CurvePoints;
		std::vector<glm::vec3> m_ControlPoints;
		bool m_ShowCurvePoints;
		bool m_ShowControlPoints;

		float m_resize_factor;
		glm::vec3 m_translate;

		std::vector<glm::vec3> m_UpdatedPointsOnCurve;
		std::vector<glm::vec3> m_UpdatedControlPoints;

		std::unique_ptr<VertexArray> m_VAO_CurvePoints;
		std::unique_ptr<VertexBuffer> m_VertexBuffer_CurvePoints;
		std::unique_ptr<IndexBuffer> m_IndexBuffer_CurvePoints;

		std::unique_ptr<VertexArray> m_VAO_ControlPoints;
		std::unique_ptr<VertexBuffer> m_VertexBuffer_ControlPoints;
		std::unique_ptr<IndexBuffer> m_IndexBuffer_ControlPoints;

		std::unique_ptr<Shader> m_Shader;
	};
}
