#pragma once

#include "Morpheus/Core/Common.h"

#include "Platform/Vulkan/VulkanCore/VulkanInstance.h"
#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"

#include "Morpheus/Renderer/RendererCore/GraphicsContext.h"

namespace Morpheus { namespace Vulkan {

	class VulkanContext : public GraphicsContext
	{
	public:
		VulkanContext() = default;
		~VulkanContext() = default;

		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void Flush() override; 

	private:
		Ref<VulkanInstance> m_Instance;
		Ref<VulkanDevice> m_Device;

		static bool s_Running;
	};

}}