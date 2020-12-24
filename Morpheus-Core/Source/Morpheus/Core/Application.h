#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Core/ApplicationBase.h"
#include "Morpheus/Core/Window.h"
#include "Morpheus/Core/LayerSystem.h"
#include "Morpheus/Core/ApplicationUnit.h"

#include "Morpheus/Events/Event.h"
#include "Morpheus/Events/ApplicationEvent.h"

#include "Morpheus/Renderer/RendererCore/GraphicsContext.h"
#include "Morpheus/Utilities/DeltaTime.h"
#include "Morpheus/Utilities/ThreadPool.h"

int main(int argc, char** argv);

namespace Morpheus {

	class Application : public ApplicationBase
	{
	public:
		Application();
		virtual ~Application() = default;

		virtual void Init() override;
		virtual void Run() override;
		virtual void Stop() override;

		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline const bool& GetRunning() { return m_Running; }
		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }

	private:
		void Render();
		void Update(const DeltaTime& _Delta);
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		Scope<Window> m_Window;
		Ref<GraphicsContext> m_Graphics;
		Vector<Ref<ApplicationUnit>> m_Units;
		Ref<ThreadPool> m_ThreadPool;

		LayerContainer m_LayerContainer;
		uint32 m_ProcessorThreads = 0;

		bool m_Running = true;
		bool m_Minimized = false;
		float32 m_LastFrameTime = 0.0;

	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

}

Morpheus::Application* CreateApplication();