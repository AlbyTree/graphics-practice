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

		void UpdateCurveControlPoints();
		void UpdateCurvePoints();
		std::vector<glm::vec3> generateCurvePointsBez(int numCurvePoints, glm::vec3 CP1, glm::vec3 CP2, glm::vec3 CP3, glm::vec3 CP4);
		
	private:

		int m_NumCurvePoints;
		std::vector<glm::vec3> m_CurvePoints;
		std::vector<glm::vec3> m_ControlPoints;
		// Used for checking when the control points positions are changed
		std::vector<glm::vec3> m_ControlPointsCopy;
		bool m_ShowCurvePoints;
		bool m_ShowControlPoints;

		float m_resize_factor;
		glm::vec3 m_translate;
		float m_curve_width;

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
