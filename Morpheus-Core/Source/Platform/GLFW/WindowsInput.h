#pragma onc

#include "Morpheus/Core/Common.h"
#include "Morpheus/Core/Input.h"

namespace Morpheus {

	class WindowsInput : public Input 
	{
	protected:
		virtual bool IsKeyPressedImpl(int32 keycode) override;

		virtual bool IsMouseButtonPressedImpl(int32 button) override;
		virtual Pair<float32, float32> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};

}