#pragma once
#include "Morpheus/Core/Common.h"

#include "Morpheus/Core/LayerSystem.h"
#include "Morpheus/Events/ApplicationEvent.h"
#include "Morpheus/Events/KeyEvent.h"
#include "Morpheus/Events/MouseEvent.h"

#include "Platform/Vulkan/VulkanImGui/VulkanImGui.h"

namespace Morpheus {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();

	private:
		float32 m_Time = 0.0f;
		VulkanImGui* m_VulkanImGui = nullptr;
	};

}