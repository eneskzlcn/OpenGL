#pragma once
#include <functional>
#include <vector>
#include <iostream>

namespace test
{

	class Test
	{
	public:
		Test() {}
		~Test() {}
		virtual void OnUpdate(float deltaTime = 0.0f) {} // this blank brackets provide that you not have to implement these virtual functions.
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currentTestPointer);
		~TestMenu();

		void OnImGuiRender() override;

		template <typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering test " << name << std::endl;
			m_Tests.push_back(std::make_pair(name, []() {return new T(); }));

		}

	private:
		Test*& m_currentTest;
		std::vector< std::pair<std::string, std::function<Test* ()>>> m_Tests;
	};

}