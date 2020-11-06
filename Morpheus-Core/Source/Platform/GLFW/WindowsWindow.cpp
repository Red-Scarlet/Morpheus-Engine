#include "morppch.h"
#include "WindowsWindow.h"

#include "Morpheus/Events/ApplicationEvent.h"
#include "Morpheus/Events/MouseEvent.h"
#include "Morpheus/Events/KeyEvent.h"

namespace Morpheus {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		//MORP_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
		MORP_CORE_ASSERT(MORP_ERROR, "GLFW ERROR");
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
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		m_Data.Rate = props.Rate;

		//MORP_CORE_INFO("{0}, Creating Window: ({1} x {2} @ {3}Hz)", props.Title, props.Width, props.Height, props.Rate);

		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			MORP_CORE_ASSERT(!success, "Could not intialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		//if (Renderer::GetAPI() == RendererAPI::API::Vulkan)
		//	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		//glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

		m_MainWindow = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);

		glfwSetWindowUserPointer(m_MainWindow, &m_Data);
		//SetVSync(true);

		// Set GLFW Callbacks
		glfwSetWindowSizeCallback(m_MainWindow, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowCloseCallback(m_MainWindow, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;

			data.EventCallback(event);
		});

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

		glfwSetCharCallback(m_MainWindow, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});

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

		glfwSetScrollCallback(m_MainWindow, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_MainWindow, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});

	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_MainWindow);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		//m_MainContext->SwapInterval(enabled);
		//m_Data.VSync = enabled;
		//CC_CORE_INFO("OpenGL: VSync {0}", enabled);
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	void WindowsWindow::SetFullscreen(bool enabled)
	{
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

		m_PrevWidth = m_Data.Width;
		m_PrevHeight = m_Data.Height;

		if(enabled) 
			glfwSetWindowMonitor(m_MainWindow, glfwGetPrimaryMonitor(), 0, 0, (1920), (1080), mode->refreshRate);
		else 
			glfwSetWindowMonitor(m_MainWindow, nullptr, 0, 0, (int)m_PrevWidth, (int)m_PrevHeight, mode->refreshRate);

		m_Data.Fullscreen = enabled;
		//CC_CORE_INFO("OpenGL: Fullscreen {0}", enabled);
	}

	bool WindowsWindow::IsFullscreen() const
	{
		return m_Data.Fullscreen;
	}

	void WindowsWindow::SetMouseActive(bool enabled)
	{		
		if(enabled)
			glfwSetInputMode(m_MainWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(m_MainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		m_Data.Mouse = enabled;
		//CC_CORE_INFO("OpenGL: Mouse {0}", enabled);
	}

	bool WindowsWindow::IsMouseActive() const
	{
		return m_Data.Mouse;
	}

}