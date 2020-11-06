#pragma once

#include "Morpheus/Core/Window.h"
#include <GLFW/glfw3.h>

namespace Morpheus {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline const uint32& GetWidth() const override { return m_Data.Width; }
		inline const uint32& GetHeight() const override { return m_Data.Height; }
		inline const uint32& GetRate() const override { return m_Data.Rate; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		virtual void SetFullscreen(bool enabled) override;
		virtual bool IsFullscreen() const override;

		virtual void SetMouseActive(bool enabled)override;
		virtual bool IsMouseActive() const override;

		inline virtual void* GetNativeWindow() const { return m_MainWindow; }

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_MainWindow;
		uint32 m_PrevWidth, m_PrevHeight;

		struct WindowData
		{
			String Title;
			uint32 Width, Height, Rate;
			bool VSync;
			bool Fullscreen;
			bool Mouse;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}