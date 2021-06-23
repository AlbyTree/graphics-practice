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

		// Perspective projection planes parameters
		float m_l, m_r, m_t, m_b, m_n, m_f;
	};
}
