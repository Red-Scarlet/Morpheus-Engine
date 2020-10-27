#include "Morppch.h"
#include "Application.h"

#include <thread>
#include "Morpheus/Utilities/DeltaTime.h"
#include <GLFW/glfw3.h>

#include "Morpheus/Renderer/Renderer.h"

namespace Morpheus {

	#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = this;
		m_TimerClass = TimerClass::GetInstance();

		WindowStruct WindowProperties;
		m_Window = Window::Create(WindowProperties);

		m_Graphics = GraphicsContext::Create();
		m_Graphics->Init();

		//RESOURCE MANAGER
		//RENDERER -> 3DRENDERER

		MORP_CORE_TRACE("[APPLICATION] Num. Threads: " + std::to_string(std::thread::hardware_concurrency()));
		m_ThreadPool = new ThreadPool(std::thread::hardware_concurrency());
	}

	void Application::Run()
	{
		Renderer::Init();
		while (m_Running)
		{
			float64 time = (float64)m_TimerClass->GetDeltaTime();
			DeltaTime Delta = time - m_LastFrameTime;

			if (Delta >= (1.00f / m_FrameLock)) {
				m_LastFrameTime = time;

				Update(Delta);
				Render();

				m_Window->OnUpdate();
				m_Window->SetUpdateStructTitle("Morpheus Engine FPS: " + std::to_string(1.00f / Delta));	
			}

			m_TimerClass->Tick();
		}
	}

	void Application::Stop()
	{
		Renderer::Shutdown();
		m_Graphics->Destory();
		m_Running = false;
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerContainer.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerContainer.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::PushFunction(Function<void()> _Func)
	{
		m_FunctionSystem.PushFunction(_Func);
	}

	void Application::Render()
	{
		for (Layer* layer : m_LayerContainer)
			layer->OnRender();
	}

	void Application::Update(const DeltaTime& _Delta)
	{
		for (Layer* layer : m_LayerContainer)
			layer->OnUpdate(_Delta);
		for (Function<void()> Func : m_FunctionSystem)
			m_ThreadPool->enqueue(Func);
		m_FunctionSystem.Reset();
	}

}