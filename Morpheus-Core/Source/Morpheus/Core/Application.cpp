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

		WindowProps WindowProperties;
		WindowProperties.Title = "Morpheus Engine Version: 0.2.0";
		m_Window = Window::Create(WindowProperties);
		m_Window->SetEventCallback(MORP_BIND_EVENT_FN(Application::OnEvent));

		m_Graphics = GraphicsContext::Create();
		m_Graphics->Init();
		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		MORP_CORE_TRACE("[APPLICATION] Num. Threads: " + std::to_string(std::thread::hardware_concurrency()));
		m_ThreadPool = new ThreadPool(std::thread::hardware_concurrency());
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float64 time = (float64)glfwGetTime();
			DeltaTime Delta = time - m_LastFrameTime;

			if (m_FrameLock == 0.00f || (Delta >= (1.00f / m_FrameLock))) {
				m_LastFrameTime = time;

				Update(Delta);
				Render();

				m_Window->OnUpdate();
			}
		}
	}

	void Application::Stop()
	{
		Renderer::Shutdown();
		m_Graphics->Destory();
		m_Running = false;
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

		for (auto it = m_LayerContainer.end(); it != m_LayerContainer.begin();)
			(*--it)->OnEvent(e);
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
		if (!m_Minimized) 
			for (Layer* layer : m_LayerContainer) 
				layer->OnRender();
			
		m_ImGuiLayer->Begin();
		for (Layer* layer : m_LayerContainer)
			layer->OnImGuiRender();
		m_ImGuiLayer->End();

		m_Graphics->End();
	}

	void Application::Update(const DeltaTime& _Delta)
	{
		for (Layer* layer : m_LayerContainer)
			layer->OnUpdate(_Delta);
		for (Function<void()> Func : m_FunctionSystem)
			m_ThreadPool->enqueue(Func);
		m_FunctionSystem.Reset();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		//Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

}