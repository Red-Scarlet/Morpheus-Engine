#pragma once

#include "Common.h"

namespace Morpheus {

	struct WindowStruct
	{
		String Title = "Morphues Engine";
		UINT32 Width = 1280, Height = 720;
	};

	class Window 
	{
	public:
		virtual ~Window() = default;
		virtual void OnUpdate() = 0;
		virtual const WindowStruct GetStruct() const = 0;

		virtual const UINT32& GetExtensionsCount() const = 0;
		virtual const FLOAT8** GetExtensionsData() const = 0;

	public:
		static Scope<Window> Create(const WindowStruct& _Struct);
	};

}