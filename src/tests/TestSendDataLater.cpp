#include "TestSendDataLater.h"
#include "../core/Renderer.h"

#include "../deps/imgui/imgui.h"
#include "../deps/glm/glm.hpp"
#include "../deps/glm/gtc/type_ptr.hpp"

namespace test
{
	TestSendDataLater::TestSendDataLater()
		: m_ShowWarningOnce(false)
	{
		unsigned int indices[] = { 0,1,2 };

        m_VAO = std::make_unique<VertexArray>();
		// Just create an uninitialized buffer on the GPU.
        m_VertexBuffer = std::make_unique<VertexBuffer>(sizeof(m_data));

        VertexBufferLayout vbLayout;
        vbLayout.AddAttrib<float>(3);

        m_VAO->AddBuffer(*m_VertexBuffer, vbLayout, false);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, sizeof(indices));

        m_Shader = std::make_unique<Shader>("res/shaders/SendDataLater.shader");
	}

	TestSendDataLater::~TestSendDataLater()
	{
	}

	void TestSendDataLater::OnUpdate(float deltaTime)
	{
	}

	void TestSendDataLater::OnRenderer()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

		if (m_VAO->GetBufferState() == NO_BUFFER ||
			m_VAO->GetBufferState() == UNINIT_BUFFER)
		{
			if (!m_ShowWarningOnce)
			{
				std::cout << "WARNING: the current vertex array doesn't have associated a buffer OR has an uninitialized buffer!"
					<< std::endl << "Refusing to draw..." << std::endl;
				m_ShowWarningOnce = true;
			}
		}
		else
		{
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
			m_ShowWarningOnce = false;
		}
	}

	void TestSendDataLater::OnImGuiRenderer()
	{
		if (ImGui::Button("Send Data"))
		{
			m_VAO->ReplaceBufferData(m_data, sizeof(m_data));
		}
	}
}
