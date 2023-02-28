#pragma once

#include <memory>

#include "Test.h"

#include "core/VertexBuffer.h"
#include "core/VertexBufferLayout.h"

namespace test
{
	class TestSendDataLater : public Test
	{
	public:
		TestSendDataLater();
		~TestSendDataLater();

		void OnUpdate(float deltaTime) override;
		void OnRenderer() override;
		void OnImGuiRenderer() override;

	private:
		const float m_data[9] = {
			 // Counter-clockwise order
			 1.0f, -1.0f, 0.0f,
			 0.0f,  1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f
		};

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;

		bool m_ShowWarningOnce;
	};
}
