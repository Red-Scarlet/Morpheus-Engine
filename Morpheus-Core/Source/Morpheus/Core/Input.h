#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Utilities/KeyCodes.h"

namespace Morpheus {

	class Input
	{
	public:
		inline static bool IsKeyPressed(int32 keycode) { return s_Instance->IsKeyPressedImpl(keycode); }

		inline static bool IsMouseButtonPressed(int32 button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static Pair<float32, float32> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }

	protected:
		virtual bool IsKeyPressedImpl(int32 keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(int32 button) = 0;

		virtual Pair<float32, float32> GetMousePositionImpl() = 0;
		virtual float32 GetMouseXImpl() = 0;
		virtual float32 GetMouseYImpl() = 0;

	private:
		static Input* s_Instance;

	};

}