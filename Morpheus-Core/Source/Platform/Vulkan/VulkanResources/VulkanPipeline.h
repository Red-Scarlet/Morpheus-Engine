#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSwapchain.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanCommand/VulkanCommandSystem.h"

#include "Platform/Vulkan/VulkanResources/VulkanDescriptorPool.h"
#include "Platform/Vulkan/VulkanResources/VulkanRenderpass.h"

#include "VulkanResource.h"

namespace Morpheus {

	struct VulkanPipelineInput
	{
	public:
		vk::ShaderModule VertModule;
		vk::ShaderModule FragModule;
	};

	class VulkanPipeline : public VulkanResource
	{
	public:
		VulkanPipeline(const VulkanPipelineInput& _Input, const vk::PipelineVertexInputStateCreateInfo& _InputState);
		virtual ~VulkanPipeline();

	private:
		virtual void VulkanCreate() override;
		virtual void VulkanDestory() override;

	public:
		const vk::Pipeline& GetPipeline() { return m_Pipeline; }
		const vk::PipelineLayout& GetLayout() { return m_PipelineLayout; }
		void SetInputState(const vk::PipelineVertexInputStateCreateInfo& _InputState) { m_InputState = _InputState; }

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanSwapchain> m_Swapchain;
		Ref<VulkanRenderpass> m_Renderpass;
		Ref<VulkanDescriptorPool> m_DescriptorPool;

		VulkanPipelineInput m_ShaderData;

		vk::Pipeline m_Pipeline;
		vk::PipelineCache m_PipelineCache;
		vk::PipelineLayout m_PipelineLayout;

		vk::PipelineVertexInputStateCreateInfo m_InputState;

	public:
		static Ref<VulkanPipeline> Make(const VulkanPipelineInput& _Input, const vk::PipelineVertexInputStateCreateInfo& _InputState);
	};

}