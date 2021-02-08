#include "morppch.h"
#include "WindowsWindow.h"

#include "Morpheus/Events/ApplicationEvent.h"
#include "Morpheus/Events/MouseEvent.h"
#include "Morpheus/Events/KeyEvent.h"

#include <string>

namespace Morpheus {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		String Message = "[GLFW] " + String(description);
		MORP_CORE_ERROR(Message);
		MORP_CORE_ASSERT(MORP_ERROR, "[GLFW] ERROR CALLBACK!");
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		MORP_PROFILE_FUNCTION();

		{
			MORP_PROFILE_SCOPE("WindowsWindow::Init->GLFWInit()");
			m_Data.Title = props.Title;
			m_Data.Width = props.Width;
			m_Data.Height = props.Height;
			m_Data.Rate = props.Rate;

			if (!s_GLFWInitialized)
			{
				// TODO: glfwTerminate on system shutdown
				int success = glfwInit();
				MORP_CORE_ASSERT(success, "Could not intialize GLFW!");
				glfwSetErrorCallback(GLFWErrorCallback);
				s_GLFWInitialized = true;
			}
		}

		{
			MORP_PROFILE_SCOPE("WindowsWindow::Init->GLFWCreate()");
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
			m_MainWindow = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
			glfwSetWindowUserPointer(m_MainWindow, &m_Data);
		}

		{
			MORP_PROFILE_SCOPE("WindowsWindow::Init->WindowSizeCallback()");
			glfwSetWindowSizeCallback(m_MainWindow, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);
			});
		}

		{
			MORP_PROFILE_SCOPE("WindowsWindow::Init->WindowCloseCallback()");
			glfwSetWindowCloseCallback(m_MainWindow, [](GLFWwindow* window)
			{

				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;

				data.EventCallback(event);
			});
		}

		{
			MORP_PROFILE_SCOPE("WindowsWindow::Init->WindowKeyCallback()");
			glfwSetKeyCallback(m_MainWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{

				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
				}
			});
		}

		{
			
			MORP_PROFILE_SCOPE("WindowsWindow::Init->WindowCharCallback()");
			glfwSetCharCallback(m_MainWindow, [](GLFWwindow* window, unsigned int keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				KeyTypedEvent event(keycode);
				data.EventCallback(event);
			});
		}

		{
			MORP_PROFILE_SCOPE("WindowsWindow::Init->WindowMouseButtonCallback()");
			glfwSetMouseButtonCallback(m_MainWindow, [](GLFWwindow* window, int button, int action, int mods)
			{

				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
				}
			});
		}

		{
			MORP_PROFILE_SCOPE("WindowsWindow::Init->WindowScrollCallback()");
			glfwSetScrollCallback(m_MainWindow, [](GLFWwindow* window, double xOffset, double yOffset)
			{

				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			});
		}

		{
			MORP_PROFILE_SCOPE("WindowsWindow::Init->WindowCursorPosCallback()");
			glfwSetCursorPosCallback(m_MainWindow, [](GLFWwindow* window, double xPos, double yPos)
			{			

				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallback(event);
			});
		}
	}

	void WindowsWindow::Shutdown()
	{
		MORP_PROFILE_FUNCTION();
		glfwDestroyWindow(m_MainWindow);
	}

	void WindowsWindow::OnUpdate()
	{
		MORP_PROFILE_FUNCTION();
		glfwPollEvents();
	}

	void WindowsWindow::SetTitle(const String& _Title)
	{
		MORP_PROFILE_FUNCTION();

		glfwSetWindowTitle(m_MainWindow, _Title.c_str());
		m_Data.Title = _Title;
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	void WindowsWindow::SetFullscreen(bool enabled)
	{
	}

	bool WindowsWindow::IsFullscreen() const
	{
		return m_Data.Fullscreen;
	}

	void WindowsWindow::SetMouseActive(bool enabled)
	{		
		if(enabled) glfwSetInputMode(m_MainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else glfwSetInputMode(m_MainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		m_Data.Mouse = enabled;
	}

	bool WindowsWindow::IsMouseActive() const
	{
		return m_Data.Mouse;
	}

}