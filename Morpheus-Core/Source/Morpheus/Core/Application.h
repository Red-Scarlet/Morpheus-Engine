#pragma once

#include "Common.h"
#include "Window.h"

int main(int argc, char** argv);

namespace Morpheus {

	class Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Run();
		void Stop();

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }

	private:
		Scope<Window> m_Window;
		bool m_Running = true;
		bool m_Minimized = false;

	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	// To be defined in CLIENT

}

Morpheus::Application* CreateApplication();