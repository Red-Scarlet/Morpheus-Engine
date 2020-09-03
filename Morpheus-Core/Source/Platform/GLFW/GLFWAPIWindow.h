#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Core/Window.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Morpheus {

	class GLFWAPIWindow : public Window
	{
	public:
		GLFWAPIWindow(const WindowStruct& _Struct);
		virtual ~GLFWAPIWindow();

		virtual void OnUpdate() override;
		virtual const WindowStruct GetStruct() const override;

		virtual const UINT32& GetExtensionsCount() const override;
		virtual const FLOAT8** GetExtensionsData() const override;

	private:
		GLFWwindow* m_Instance;
		WindowStruct m_WindowStruct;
		UINT32 m_ExtensionsCount;
		const FLOAT8** m_ExtensionsData;
	};

}