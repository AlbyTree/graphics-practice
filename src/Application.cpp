#pragma region INCLUDES
#include <iostream>
#include <fstream>

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "core/Renderer.h"
#include "core/Shader.h"

#include "deps/imgui/imgui.h"
#include "deps/imgui/imgui_impl_glfw.h"
#include "deps/imgui/imgui_impl_opengl3.h"

#include "tests/Test.h"
#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestProjSteps.h"
#include "tests/TestSendDataLater.h"
#include "tests/TestBezCurve.h"
#include "tests/TestMesh.h"
#pragma endregion

int main(void)
{
#pragma region SETUP
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Instead of drawing our frames the moment they are ready
    // (and so if it happens during the monitor refresh rate interval
    // it tries to draw it while it's trying to draw the previous frame)
    // it waits for the refresh rate to end and then use the new frame
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error: failed to initialize GLEW(did you previously create a valid OpenGL context ?)!" << std::endl;
        return -1;
    }

    GLCall(const GLubyte* driverVers = glGetString(GL_VERSION));
    std::cout << "OpenGL Driver VERSION: " << driverVers << std::endl;
    std::cout << std::endl;

    {
        // Allow to use the alpha pixel values to "blend" the pixels
        // i.e. to reduce opacity of those pixels on top of the pixels with alpha values.
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        Renderer renderer;

        //----- IMGUI SETUP -----//
		const char* glsl_version = "#version 130";

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::StyleColorsDark();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);

        //----- IMGUI SETUP FINISHED -----//

        test::Test* currentTest = nullptr;
        test::TestMenu* testMenu = new test::TestMenu(currentTest);
        currentTest = testMenu;
#pragma endregion

        testMenu->RegisterTest<test::TestClearColor>("Clear Color");
        testMenu->RegisterTest<test::TestTexture2D>("Texture 2D");
        testMenu->RegisterTest<test::TestProjSteps>("Projection Steps");
        testMenu->RegisterTest<test::TestSendDataLater>("Send Data Later");
        testMenu->RegisterTest<test::TestBezCurve>("Bezier Curve");
        testMenu->RegisterTest<test::TestMesh>("Mesh Test");

#pragma region GAME_LOOP
        while (!glfwWindowShouldClose(window))
        {
            GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
            renderer.Clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();
            
            if (currentTest)
            {
                currentTest->OnUpdate(0.0f);
                currentTest->OnRenderer();
                ImGui::Begin("Test");
                if (currentTest != testMenu && ImGui::Button("<-"))
                {
                    delete currentTest;
                    currentTest = testMenu;
                }
                currentTest->OnImGuiRenderer();
                ImGui::End();
            }

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        delete currentTest;
        if (currentTest != testMenu)
            delete testMenu;
    }
#pragma endregion

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
