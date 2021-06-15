#include "Test.h"

#include "../deps/imgui/imgui.h"

namespace test
{
	TestMenu::TestMenu(Test*& currentTestPtr)
		: m_CurrentTest(currentTestPtr)
	{
	}

	void TestMenu::OnImGuiRenderer()
	{
		for (auto& test : m_Tests)
		{
			if (ImGui::Button(test.first.c_str()))
				m_CurrentTest = test.second();
		}
	}
}