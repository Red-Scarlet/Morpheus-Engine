#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Renderer/RendererCore/RendererAPI.h"

#include "Platform/VULKAN/VulkanCommand.h"
#include "Platform/VULKAN/VulkanSynchronization.h"
#include "Platform/VULKAN/VulkanResources/VulkanDescriptorPool.h"

//#include "Platform/VULKAN/VulkanSwapchain.h"
//#include "Platform/VULKAN/VulkanDevice.h"

namespace Morpheus {

	class VulkanRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void Flush() override;

		virtual void SetCompile(const bool& _Value) override;
		virtual void SetReady(const bool& _Value) override;

	private:
		void Resize();

	private:
		Ref<VulkanCommand> m_Command;
		Ref<VulkanSynchronization> m_Sync;
		//Ref<VulkanDescriptorPool> m_DescriptorPool;

		bool m_CompileCommands = false;
		bool m_ReadyRender = false;
	};


}