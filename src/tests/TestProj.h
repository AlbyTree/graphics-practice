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
		TestPerspProjSteps(const glm::vec3& point_e, float l, float r, float t, float b, float n, float f);
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

		// Perspective projection planes parameters
		float m_l, m_r, m_t, m_b, m_n, m_f;

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_TranslationA, m_TranslationB;
	};
}
