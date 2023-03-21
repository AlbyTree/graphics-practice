#include "TestPoints.h"

#include "core/Renderer.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include <iostream>

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
                static_cast<TestPoints*>(glfwGetWindowUserPointer(window))->OnMouseLeftClick(window, button, action, mods);
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
    
    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(10.f);
}

void test::TestPoints::OnRenderer()
{
    Renderer r;
    
    glm::mat4 model, view, projection;
    glm::vec3 modelScale(100);
    model = glm::scale(glm::mat4(1.f), modelScale);
    view = glm::mat4(1.f);
    projection = glm::ortho(-480.0f, 480.0f, -270.0f, 270.0f);

    m_Shader->Bind();
    
    m_Shader->SetUniformMat4f("u_Model", model);
    m_Shader->SetUniformMat4f("u_View", view);
    m_Shader->SetUniformMat4f("u_Projection", projection);

    r.Draw(m_Mesh.GetVAO(), m_Mesh.GetIB(), *m_Shader, Primitive::POINTS);
    
    m_Shader->Unbind();
}

void test::TestPoints::OnMouseLeftClick(GLFWwindow* window, int button, int action, int mode)
{
}
