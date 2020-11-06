#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Events/Event.h"

namespace Morpheus {

	struct WindowProps
	{
		String Title;
		uint32 Width;
		uint32 Height;
		uint32 Rate;

		WindowProps(const String& title = "Concealing Engine",
			const uint32& width = 1280,
			const uint32& height = 720,
			const uint32& rate = 60)
			: Title(title), Width(width), Height(height), Rate(rate)
		{
		}
	};

	// Interface representing a desktop system based Window
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual const uint32& GetWidth() const = 0;
		virtual const uint32& GetHeight() const = 0;
		virtual const uint32& GetRate() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void SetFullscreen(bool enabled) = 0;
		virtual bool IsFullscreen() const = 0;

		virtual void SetMouseActive(bool enabled) = 0;
		virtual bool IsMouseActive() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowProps& props = WindowProps());
	};

}