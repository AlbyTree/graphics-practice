#pragma once

#include <memory>

#include "Test.h"

#include "../core/VertexBuffer.h"
#include "../core/VertexBufferLayout.h"
#include "../core/Texture.h"

namespace test
{
	class TestPerspProjSteps : public Test
	{
	public:
		TestPerspProjSteps();
		~TestPerspProjSteps();

		void OnUpdate(float deltaTime) override;
		void OnRenderer() override;
		void OnImGuiRenderer() override;

		void ProjectPointToNearPlane();
		void MapPointToClipSpace();
		void NDCTransf();
		void ProjectPoint();

	private:

		// In the following order: 
		// The point in eye space
		// The point projected on the near plane
		// The point clipped in homogeneous space 
		// The point in NDC space
		glm::vec3 m_point_e;
		glm::vec3 m_point_p;
		glm::vec4 m_point_c;
		glm::vec3 m_point_NDC;

		// Perspective projection planes parameters:
		// the near and far plane parameters MUST be positive values
		float m_l, m_r, m_t, m_b, m_n, m_f;

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
	};
}
