#include "Morppch.h"
#include "GLFWAPIWindow.h"

namespace Morpheus {

	GLFWAPIWindow::GLFWAPIWindow(const WindowStruct& _Struct)
		: m_WindowStruct(_Struct)
	{
		glfwInit();
		
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		m_Instance = glfwCreateWindow(m_WindowStruct.Width, m_WindowStruct.Height, m_WindowStruct.Title.c_str(), nullptr, nullptr);

		vkEnumerateInstanceExtensionProperties(nullptr, &m_ExtensionsCount, nullptr);
		m_ExtensionsData = glfwGetRequiredInstanceExtensions(&m_ExtensionsCount);

		
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

	const UINT32& GLFWAPIWindow::GetExtensionsCount() const
	{
		return m_ExtensionsCount;
	}

	const FLOAT8** GLFWAPIWindow::GetExtensionsData() const
	{
		return m_ExtensionsData;
	}

}