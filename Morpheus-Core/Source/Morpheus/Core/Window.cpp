#include "Morppch.h"
#include "Window.h"

#include "Platform/GLFW/GLFWAPIWindow.h"

namespace Morpheus {

	Scope<Window> Window::Create(const WindowStruct& _Struct)
	{
		return CreateScope<GLFWAPIWindow>(_Struct);
	}

}

