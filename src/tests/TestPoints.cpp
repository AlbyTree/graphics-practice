#include "TestPoints.h"

#include "core/Renderer.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_projection.hpp"

test::TestPoints::TestPoints()
{
    Initialize(nullptr);
}

test::TestPoints::TestPoints(GLFWwindow* window)
{
    Initialize(window);
}

test::TestPoints::~TestPoints()
{
    glDisable(GL_PROGRAM_POINT_SIZE);
    glfwSetWindowUserPointer(m_Window, nullptr);
    glfwSetMouseButtonCallback(m_Window, nullptr);
}

void test::TestPoints::Initialize(GLFWwindow* window) noexcept
{
    m_Window = window;

    glfwSetWindowUserPointer(m_Window, this);
    glfwSetMouseButtonCallback(m_Window,
        [](GLFWwindow* window, int button, int action, int mods)
        {
            if (glfwGetWindowUserPointer(window))
                static_cast<TestPoints*>(glfwGetWindowUserPointer(window))->OnMouseButton(window, button, action, mods);
        });
    
    std::vector<MeshData> data = {
        {{-1,0,0}},
        {{0,1,0}},
        {{1,0,0}},
        {{0,0,0}}
    };
    
    m_Mesh.SetVertices(std::move(data));
    m_Mesh.SetIndices({0,1,2,3});

    m_Shader = std::make_unique<Shader>("res/shaders/Default.shader");
    
    glm::vec3 modelScale(100);
    m_Model = glm::scale(glm::mat4(1.f), modelScale);
    m_View = glm::mat4(1.f);
    m_View = glm::lookAt(glm::vec3(0,0,5), glm::vec3(0,0,0), glm::vec3(0,1,0));
    m_Projection = glm::ortho(-480.0f, 480.0f, -270.0f, 270.0f, 0.1f, 100.f);
    m_Viewport = glm::vec4(0.f, 0.f, 960.f, 540.f);
    
    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(10.f);
}

void test::TestPoints::OnRenderer()
{
    Renderer r;
    
    m_Shader->Bind();
    
    m_Shader->SetUniformMat4f("u_Model", m_Model);
    m_Shader->SetUniformMat4f("u_View", m_View);
    m_Shader->SetUniformMat4f("u_Projection", m_Projection);

    r.Draw(m_Mesh.GetVAO(), m_Mesh.GetIB(), *m_Shader, Primitive::POINTS);
    
    m_Shader->Unbind();
}

void test::TestPoints::OnMouseButton(GLFWwindow* window, int button, int action, int mode)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
    }
}
