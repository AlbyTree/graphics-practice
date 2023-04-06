#include "TestPoints.h"

#include "core/Renderer.h"
#include "core/Utils.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_projection.hpp"
#include "glm/gtx/normalize_dot.hpp"

using compgraphutils::Ray;
using compgraphutils::RayIntersects;

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
    glfwSetCursorPosCallback(m_Window,
        [](GLFWwindow* window, double xpos, double ypos)
        {
            if (glfwGetWindowUserPointer(window))
                static_cast<TestPoints*>(glfwGetWindowUserPointer(window))->OnCursorPosition(window, xpos, ypos);
        });
    
    m_Mesh.AddPositions({
        {-1,0,0},
        {0,1,0},
        {1,0,0},
        {0,0,0}});
        
	m_Mesh.AddColor({1,1,1,1});
	m_Mesh.AddColor({1,1,1,1});
	m_Mesh.AddColor({1,1,1,1});
	m_Mesh.AddColor({1,1,1,1});
	
    m_Mesh.AddIndices({0,1,2,3});

	m_Mesh.UpdateBuffers();

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
    	if (!selected)
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
			
			indexSelectedVertex = RayIntersects(mouseRay, worldMeshPositions);
			if (indexSelectedVertex >= 0)
			{
				m_Mesh.SetVertexColor(indexSelectedVertex, {0.2f, 0.3f, 0.5f, 1.f});
				m_Mesh.UpdateBuffers();
				selected = true;
				drag = true;
				mousePrev = glm::vec3(xPos, yPos, 1.f);
			}
    	}
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
		if (selected)
		{
			selected = false;
			m_Mesh.SetVertexColor(indexSelectedVertex, {1.0f, 1.0f, 1.0f, 1.f});
			m_Mesh.UpdateBuffers();
			indexSelectedVertex = -1;
			mousePrev = glm::vec3(0);
		}
    	if (drag)
    	{
    		drag = false;
    		firstDrag = true;
    	}
    }
}

void test::TestPoints::OnCursorPosition(GLFWwindow* window, double xPos, double yPos)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Remember where the mouse was before dragging
		static glm::vec3 mouseObjPrev(0);
		if (selected && drag)
		{
			// Unproject prev mouse and current mouse in object space
			if (firstDrag)
			{
				mouseObjPrev = glm::unProject(mousePrev, m_View * m_Model, m_Projection, m_Viewport);
				// Window Y axis is opposite of world Y axis
				mouseObjPrev.y = -mouseObjPrev.y;
				firstDrag = false;
			}
			
			glm::vec3 mouseObjCurr = glm::unProject({xPos, yPos, 1.f}, m_View * m_Model, m_Projection, m_Viewport);
			// Window Y axis is opposite of world Y axis
			mouseObjCurr.y = -mouseObjCurr.y;

			// Find mouse delta in object space
			glm::vec3 mouseDelta = mouseObjCurr - mouseObjPrev;

			mouseObjPrev = mouseObjCurr;

			//TODO: Find right,up vector (i.e. orientation) and apply delta in that directions
			//TODO: Apply delta in world space, not in object space
			// Apply delta to point
			auto pos = m_Mesh.GetPositions()[indexSelectedVertex];
			m_Mesh.SetVertexPosition(indexSelectedVertex, {pos.x + mouseDelta.x, pos.y + mouseDelta.y, pos.z});
			m_Mesh.UpdateBuffers();
		}
	}
}
