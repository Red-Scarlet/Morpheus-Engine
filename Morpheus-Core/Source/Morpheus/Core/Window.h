#pragma once

#include "Common.h"

namespace Morpheus {

	struct WindowStruct
	{
		String Title = "Morphues Engine";
		uint32 Width = 1280, Height = 720;
	};

	class Window 
	{
	public:
		virtual ~Window() = default;
		virtual void OnUpdate() = 0;
		virtual const WindowStruct GetStruct() const = 0;
		virtual void SetUpdateStructTitle(const String& Title) = 0;

		virtual const uint32& GetExtensionsCount() const = 0;
		virtual const float8** GetExtensionsData() const = 0;

		virtual void* GetWindowCore() = 0;

	public:
		static Scope<Window> Create(const WindowStruct& _Struct);
	};

}