#pragma once

#include "Common.h"
#include "Window.h"

#include "Morpheus/Renderer/RendererInstance.h"
#include "Morpheus/Core/LayerSystem.h"
#include "Morpheus/Core/FunctionSystem.h"

#include "Morpheus/Utilities/ThreadPool.h"
#include "Morpheus/Utilities/TimerClass.h"

int main(int argc, char** argv);

namespace Morpheus {

	class Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Run();
		void Stop();
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PushFunction(Function<void()> _Func);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }

	private:
		Scope<Window> m_Window;
		RendererInstance* m_RenderInstance;
		LayerContainer m_LayerContainer;
		FunctionSystem m_FunctionSystem;
		ThreadPool* m_ThreadPool;
		TimerClass* m_TimerClass;

		bool m_Running = true;
		bool m_Minimized = false;
		FLOAT64 m_LastFrameTime = 0.00f;

	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

}

Morpheus::Application* CreateApplication();