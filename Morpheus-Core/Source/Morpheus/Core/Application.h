#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Core/Base.h"
#include "Morpheus/Core/Window.h"
#include "Morpheus/Core/LayerSystem.h"
#include "Morpheus/Core/AppUnit.h"

#include "Morpheus/Events/Event.h"
#include "Morpheus/Events/ApplicationEvent.h"

#include "Morpheus/Renderer/RendererCore/GraphicsContext.h"
#include "Morpheus/Utilities/DeltaTime.h"
#include "Morpheus/Utilities/ThreadPool.h"

#ifdef MORP_DIST
int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
);
#else
int main(int argc, char** argv);
#endif

namespace Morpheus {

	class Application : public Base
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
		Ref<ThreadPool> m_ThreadPool;
		LayerContainer m_LayerContainer;
		uint32 m_NumThreads = 0;

		bool m_Running = true;
		bool m_Minimized = false;
		float32 m_LastFrameTime = 0.0;
		Vector<Ref<AppUnit>> m_AppUnits;
	private:
		static Application* s_Instance;
		#ifdef MORP_DIST
		friend int CALLBACK ::WinMain(
			_In_ HINSTANCE hInstance,
			_In_opt_ HINSTANCE hPrevInstance,
			_In_ LPSTR     lpCmdLine,
			_In_ int       nCmdShow
		);
		#else
		friend int ::main(int argc, char** argv);
		#endif
	};

}

Morpheus::Application* CreateApplication();