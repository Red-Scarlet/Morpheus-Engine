#include "morppch.h"

#include "Window.h"
#include "Platform/GLFW/WindowsWindow.h"

namespace Morpheus {

	Scope<Window> Window::Create(const WindowProps& props)
	{
		return CreateScope<WindowsWindow>(props);
		return nullptr;
	}

}