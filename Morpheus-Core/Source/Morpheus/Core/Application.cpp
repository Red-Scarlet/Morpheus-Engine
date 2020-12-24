#include "Morppch.h"
#include "Application.h"

#include "Morpheus/Utilities/DeltaTime.h"
#include "Morpheus/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Morpheus {

	#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		MORP_PROFILE_FUNCTION();

		{
			MORP_PROFILE_SCOPE("Applicaton::Application()->WindowInit");

			s_Instance = this;
			WindowProps WindowProperties;
			WindowProperties.Title = "Morpheus Engine";
			m_Window = Window::Create(WindowProperties);
		}
	
		std::stringstream ss;
		ss << std::this_thread::get_id();
		MORP_CORE_SPECIAL("[APPLICATION] ThreadID: " + ss.str());

		Init();
	}

	void Application::Init()
	{
		MORP_PROFILE_FUNCTION();

		{			
			MORP_PROFILE_SCOPE("Applicaton::Application()->WindowInit");
			m_Window->SetEventCallback(MORP_BIND_EVENT_FN(Application::OnEvent));
		}

		{
			MORP_PROFILE_SCOPE("Applicaton::Application()->ThreadPoolInit");
			m_ProcessorThreads = std::thread::hardware_concurrency() - 1;
			m_ThreadPool = ThreadPool::Create(m_ProcessorThreads);
		}
		
		{
			MORP_PROFILE_SCOPE("Applicaton::Application()->GraphicsInit");
			m_Graphics = GraphicsContext::Create();
			m_Graphics->Init();
		}

		{
			MORP_PROFILE_SCOPE("Applicaton::Application()->ApplicationThreadsInit");

			while (m_Units.size() != m_ProcessorThreads) {
				m_Units.push_back(ApplicationUnit::Create(m_Units.size()));
				m_Units[m_Units.size() - 1]->SetGraphicsContext(m_Graphics);
				m_Units[m_Units.size() - 1]->Init();
			}
		}

		Renderer::Init();
	}

	void Application::Run()
	{
		MORP_PROFILE_FUNCTION();

		DeltaTime Delta = 0;
		float32 Time = 0;

		MORP_CORE_INFO("[APPLICATION] Running...!");

		for (Ref<ApplicationUnit>& unit : m_Units)
			m_ThreadPool->Enqueue([unit] { unit->Run(); });

		while (m_Running)
		{
			Time = (float32)glfwGetTime();
			Delta = Time - m_LastFrameTime;
			m_LastFrameTime = Time;

			Update(Delta);
			Render();

			#ifdef MORP_DEBUG
			m_Window->SetTitle("Morpheus Engine: CPU: " + ToString(Delta * 1000) + " ms");
			#endif

			m_Window->OnUpdate();
		}

		for (Ref<ApplicationUnit> unit : m_Units)
			unit->Stop();

		m_ThreadPool->Stop();
	}

	void Application::Stop()
	{
		MORP_PROFILE_FUNCTION();

		m_Running = false;
	
		Renderer::Shutdown();
		m_Graphics->Shutdown();
	}

	void Application::OnEvent(Event& e)
	{
		MORP_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

		for (auto it = m_LayerContainer.end(); it != m_LayerContainer.begin();)
			(*--it)->OnEvent(e);
	}

	void Application::PushLayer(Layer* layer)
	{
		MORP_PROFILE_FUNCTION();

		m_LayerContainer.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		MORP_PROFILE_FUNCTION();

		m_LayerContainer.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::Render()
	{
		MORP_PROFILE_FUNCTION();

		if (!m_Minimized) {

			{
				MORP_PROFILE_SCOPE("Application::Render OnLayer");
				for (Layer* layer : m_LayerContainer)
					layer->OnRender();
			}

			#ifdef MORP_DEBUG
			{
				//MORP_PROFILE_SCOPE("Application::Render OnImGui");
				//m_ImGuiLayer->Begin();
				//for (Layer* layer : m_LayerContainer)
				//	layer->OnImGuiRender();
				//m_ImGuiLayer->End();
			}
			#endif
		}
	}

	void Application::Update(const DeltaTime& _Delta)
	{
		MORP_PROFILE_FUNCTION();

		for (Layer* layer : m_LayerContainer)
			layer->OnUpdate(_Delta);
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		MORP_PROFILE_FUNCTION();

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
		return false;
	}

}