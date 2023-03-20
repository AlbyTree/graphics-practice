#pragma once

#include "Test.h"
#include "core/Mesh.h"

namespace test
{
    class TestMesh : public Test
    {
    public:
        TestMesh();

        void OnRenderer() override;

    private:
        Mesh m_Mesh;
        std::unique_ptr<Shader> m_Shader;
    };
}
