#pragma once

#include "Test.h"
#include "core/Mesh.h"
#include "deps/glm/glm.hpp"
#include "deps/GL/glew.h"
#include "deps/GLFW/glfw3.h"

namespace test
{
    class TestPoints : public Test
    {
    public:
        TestPoints();
        TestPoints(GLFWwindow* window);
        ~TestPoints() override;

        void Initialize(GLFWwindow* window) noexcept;

        void OnRenderer() override;
        void OnMouseButton(GLFWwindow* window, int button, int action, int mode);

    private:
        GLFWwindow* m_Window;
        Mesh m_Mesh;
        std::unique_ptr<Shader> m_Shader;
        glm::mat4 m_Model, m_View, m_Projection;
        glm::vec4 m_Viewport;
    };
}
