#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Core/Window.h"

#include <GLFW/glfw3.h>

namespace Morpheus {

	class GLFWWindow : public Window
	{
	public:
		GLFWWindow(const WindowStruct& _Struct);
		virtual ~GLFWWindow();

		virtual void OnUpdate() override;
		virtual const WindowStruct GetStruct() const override;

	private:
		GLFWWindow* m_Instance;
		WindowStruct m_WindowStruct;

	};

}