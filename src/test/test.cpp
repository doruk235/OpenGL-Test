#include "imgui/imgui.h"
#include "test.h"
#include <string>
#include <iostream>

namespace test{


	test::TestMenu::TestMenu(Test*& currentTestPointer)
		:m_CurrentTest(currentTestPointer)
	{

	}
	void test::TestMenu::OnImGuiRender()
	{
		for (auto& test : m_Tests) {
			if (ImGui::Button(test.first.c_str()))
				m_CurrentTest = test.second();
		}
	}
}

