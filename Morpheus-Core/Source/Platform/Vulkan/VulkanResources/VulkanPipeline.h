#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSwapchain.h"
#include "Platform/Vulkan/VulkanResources/VulkanCommand.h"
#include "Platform/Vulkan/VulkanResources/VulkanDescriptorPool.h"
#include "Platform/Vulkan/VulkanUnknown/VulkanRenderpass.h"

namespace Morpheus {

	struct VulkanPipelineInput
	{
	public:
		vk::ShaderModule VertModule;
		vk::ShaderModule FragModule;
	};

	class VulkanPipeline
	{
	public:
		VulkanPipeline(const VulkanPipelineInput& _Input, const vk::PipelineVertexInputStateCreateInfo& _InputState);
		virtual ~VulkanPipeline();
		void Destory();

		const uint32& GetID() { return m_ID; }
		void SetID(const uint32& _ID) { m_ID = _ID; }

		const vk::Pipeline& GetPipeline() { return m_Pipeline; }
		const vk::PipelineLayout& GetLayout() { return m_PipelineLayout; }

		void SetInputState(const vk::PipelineVertexInputStateCreateInfo& _InputState) { m_InputState = _InputState; }

	private:
		void CreatePipeline();

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

		uint32 m_ID = 0;

	public:
		static Ref<VulkanPipeline> VulkanCreate(const VulkanPipelineInput& _Input, const vk::PipelineVertexInputStateCreateInfo& _InputState);
	};

}