#include "Morppch.h"
#include "Window.h"

#include "Platform/GLFW/GLFWWindow.h"

namespace Morpheus {

	Scope<Window> Window::Create(const WindowStruct& _Struct)
	{
		return CreateScope<GLFWWindow>(_Struct);
	}

}

