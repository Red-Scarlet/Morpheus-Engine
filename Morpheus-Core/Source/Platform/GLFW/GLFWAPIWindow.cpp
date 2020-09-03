#include "Morppch.h"
#include "GLFWAPIWindow.h"

namespace Morpheus {

	GLFWAPIWindow::GLFWAPIWindow(const WindowStruct& _Struct)
		: m_WindowStruct(_Struct)
	{
		glfwInit();
		
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		m_Instance = glfwCreateWindow(m_WindowStruct.Width, m_WindowStruct.Height, m_WindowStruct.Title.c_str(), nullptr, nullptr);
	}

	GLFWAPIWindow::~GLFWAPIWindow()
	{
		glfwDestroyWindow(m_Instance);
		glfwTerminate();
	}

	void GLFWAPIWindow::OnUpdate()
	{
		glfwPollEvents();
	}

	const WindowStruct GLFWAPIWindow::GetStruct() const
	{ return m_WindowStruct; }

}