#include "Morppch.h"
#include "Application.h"

namespace Morpheus {

	#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = this;

		WindowStruct WindowProperties;
		m_Window = Window::Create(WindowProperties);
	}

	void Application::Run()
	{
		while (m_Running)
		{
			// ...

			m_Window->OnUpdate();
		}	
	}

	void Application::Stop()
	{
		m_Running = false;
	}

}