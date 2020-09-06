#include "Morppch.h"
#include "Application.h"

#include <thread>
#include "Morpheus/Utilities/DeltaTime.h"
#include <GLFW/glfw3.h>

namespace Morpheus {

	#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		s_Instance = this;
		m_TimerClass = TimerClass::GetInstance();

		WindowStruct WindowProperties;
		m_Window = Window::Create(WindowProperties);
		m_GraphicsContext = GraphicsContext::Create();
		//RESOURCE MANAGER
		//RENDERER -> 3DRENDERER

		MORP_CORE_TRACE("[APPLICATION] Num. Threads: " + std::to_string(std::thread::hardware_concurrency()));
		m_ThreadPool = new ThreadPool(std::thread::hardware_concurrency());
	}

	void Application::Run()
	{
		while (m_Running)
		{
			FLOAT64 time = (FLOAT64)m_TimerClass->GetDeltaTime();
			DeltaTime Delta = time - m_LastFrameTime;
			m_LastFrameTime = time;

			m_TimerClass->Tick();

			Update(Delta);
			Render();

			m_Window->OnUpdate();

			m_Window->SetUpdateStructTitle("Morpheus Engine FPS: " + std::to_string(1.00f / Delta));
		}

		m_GraphicsContext->Stop();

	}

	void Application::Stop()
	{
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
		m_GraphicsContext->Draw();
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