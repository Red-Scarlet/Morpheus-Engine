#include "Morppch.h"
#include "GLFWWindow.h"

namespace Morpheus {

	GLFWWindow::GLFWWindow(const WindowStruct& _Struct)
		: m_WindowStruct(_Struct)
	{

	}

	GLFWWindow::~GLFWWindow()
	{
	}

	void GLFWWindow::OnUpdate()
	{
	}

	const WindowStruct GLFWWindow::GetStruct() const
	{ return m_WindowStruct; }

}