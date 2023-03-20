#pragma once

#include <vector>
#include <functional>
#include <iostream>
#include "deps/GL/glew.h"
#include "deps/GLFW/glfw3.h"

namespace test
{
	class Test
	{
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRenderer() {}
		virtual void OnImGuiRenderer() {}
	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currentTestPtr);

		void OnImGuiRenderer() override;

		template <typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering test " << name << std::endl;
			m_Tests.push_back(std::make_pair(name, []() { return new T(); }));
		}
		
		template <typename T>
		void RegisterTest(const std::string& name, GLFWwindow* window)
		{
			std::cout << "Registering test " << name << std::endl;
			m_Tests.push_back(std::make_pair(name, [window]() { return new T(window); }));
		}
	
	private:
		Test*& m_CurrentTest;
		std::vector< std::pair<std::string, std::function<Test* ()>> > m_Tests;
	};
}
