#include "Test.h"
#include "imgui/imgui.h"

namespace test
{
	TestMenu::TestMenu(Test*& currentTestPointer)
		:m_currentTest(currentTestPointer)
	{

	}

	TestMenu::~TestMenu()
	{

	}

	void TestMenu::OnImGuiRender()
	{
		for (auto& s_Test : m_Tests)
		{
			if (ImGui::Button(s_Test.first.c_str()))
			{
				m_currentTest = s_Test.second();
			}
		}
	}

}