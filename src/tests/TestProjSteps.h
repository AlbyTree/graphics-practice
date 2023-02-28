#pragma once

#include <memory>

#include "Test.h"

#include "core/VertexBuffer.h"
#include "core/Texture.h"

namespace test
{
	class TestProjSteps : public Test
	{
	public:
		TestProjSteps();
		~TestProjSteps();

		void OnUpdate(float deltaTime) override;
		void OnRenderer() override;
		void OnImGuiRenderer() override;

	private:

		glm::mat3 m_Data;
		// Eye-Space data stored application side
		glm::mat3 m_UpdatedData;

		// Perspective projection planes parameters:
		// the near and far plane parameters MUST be positive values
		float m_l, m_r, m_t, m_b, m_n, m_f;
		// Perspective projection parameters
		float m_fov, m_ar;
		glm::mat4 m_Projection;

		enum TypeOfProjection { ORTHO_PROJ = 0, PERSPECTIVE_PROJ = 1};
		TypeOfProjection m_TypeOfProj;

		// "Projection" intermediate transformed data stored application side
		glm::mat3 m_ProjectedData;
		glm::mat3x4 m_ClippedData; 
		glm::mat3 m_NDCData;

		// Additional transformations
		glm::vec3 m_Translation;
		glm::vec2 m_Resize;
		float m_ScalingFactor;

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
	};
}
