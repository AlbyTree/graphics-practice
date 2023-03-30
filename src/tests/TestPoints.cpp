#include "TestPoints.h"

#include "core/Renderer.h"
#include "core/Utils.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_projection.hpp"
#include "glm/gtx/normalize_dot.hpp"

using compgraphutils::Ray;
using compgraphutils::AABB;

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
    glm::vec3 modelTrans(0,0,-5);
    m_Model = glm::translate(glm::mat4(1.f), modelTrans) * glm::scale(glm::mat4(1.f), modelScale);
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
        double xPos = -1, yPos = -1;
        glfwGetCursorPos(window, &xPos, &yPos);

    	// OpenGL window depth goes from 0 (near plane) to 1 (far plane)
        glm::vec3 mousePointNear = glm::unProject({xPos, yPos, 0.f}, m_View, m_Projection, m_Viewport);
        glm::vec3 mousePointFar = glm::unProject({xPos, yPos, 1.f}, m_View, m_Projection, m_Viewport);
    	// Window Y axis is opposite of world Y axis
    	mousePointNear.y = -mousePointNear.y;
    	mousePointFar.y = -mousePointFar.y;
        Ray mouseRay(mousePointNear, mousePointFar - mousePointNear);
        
        std::vector<glm::vec3> worldMeshPositions;
        for (const auto& pos : m_Mesh.GetPositions())
        {
            worldMeshPositions.push_back(m_Model * glm::vec4(pos.x, pos.y, pos.z, 1.f));
        }
        worldMeshPositions.resize(m_Mesh.GetNumVertices());
        
        unsigned int i = 0;
        int indexCloserVertex = -1;
        float tCurrent = -1.f;
        float distCurrent = -1.f;
        float distMin = std::numeric_limits<float>::max();
        for (const auto& meshPoint : worldMeshPositions)
        {
			//TODO: AABB ray as class member and find heuristic value
           AABB aabb(meshPoint, 5.f);
           if (compgraphutils::RayCast(mouseRay, aabb, tCurrent))
           {
               auto hitPoint = glm::vec3(mouseRay.position + mouseRay.direction * tCurrent);
               distCurrent = glm::length(meshPoint - hitPoint);
           		// If more points are close together,
           		// select the closest one to the mouse
               if (distCurrent <= distMin)
               {
                   indexCloserVertex = i;
                   distMin = distCurrent;
               }
           }
           i++; 
        }
        if (indexCloserVertex >= 0)
        {
            m_Mesh.SetVertexColor(indexCloserVertex, {0.2f, 0.3f, 0.5f, 1.f});
        }
    }
}
