#include "TestTexture2D.h"
#include "core/VertexBufferLayout.h"
#include "core/Renderer.h"

#include "deps/imgui/imgui.h"
#include "deps/glm/glm.hpp"
#include "deps/glm/gtc/matrix_transform.hpp"

namespace test
{
	TestTexture2D::TestTexture2D()
        : m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)), 
        m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))), 
        m_TranslationA(glm::vec3(200.0f, 200.0f, 0.0f)), 
        m_TranslationB(glm::vec3(400.0f, 200.0f, 0.0f))
	{
        float positions[] = {
            // 1st Triangle of square face
            // Positions    // Texture Positions
            -50.0f, -50.0f,   0.0f, 0.0f, // 0 
             50.0f, -50.0f,   1.0f, 0.0f, // 1
             50.0f,  50.0f,   1.0f, 1.0f, // 2
            // Unique vertices for a square
            -50.0f,  50.0f,   0.0f, 1.0f // 3
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 0, 3
        };

        // Allow to use the alpha pixel values to "blend" the pixels
        // i.e. to reduce opacity of those pixels on top of the pixels with alpha values.
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        m_VAO = std::make_unique<VertexArray>();
        m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout vbLayout;
        // Vertex Positions
        vbLayout.AddAttrib<float>(2);
        // Texture Positions
        vbLayout.AddAttrib<float>(2);
        m_VAO->AddBuffer(*m_VertexBuffer, vbLayout, true);
        m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

        m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
        m_Shader->Bind();
        m_Shader->SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

        m_Texture = std::make_unique<Texture>("res/textures/ChernoLogo.png");
        m_Shader->SetUniform1i("u_Texture", 0);
	}

	TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}

	void TestTexture2D::OnRenderer()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

        Renderer renderer;

        m_Texture->Bind();

        {
            // Move object to the x-left from its initial position.
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }

        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
            glm::mat4 mvp = m_Proj * m_View * model;
            m_Shader->Bind();
            m_Shader->SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
	}

	void TestTexture2D::OnImGuiRenderer()
	{
        ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
