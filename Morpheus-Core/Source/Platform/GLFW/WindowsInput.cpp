#include "morppch.h"
#include "Morpheus/Core/Application.h"
#include "WindowsInput.h"

#include <GLFW/glfw3.h>

namespace Morpheus {

	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	float WindowsInput::GetMouseXImpl()
	{
		/*
			UPDATE TO C++17 for latest feature

			auto[x, y] = GetMousePositionImpl();
			return x;
		*/

		auto [x, y] = GetMousePositionImpl();
		return x;

		//auto v = GetMousePositionImpl();
		//return std::get<0>(v);
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePositionImpl();
		return y;

		//auto v = GetMousePositionImpl();
		//return std::get<1>(v);
	}
}