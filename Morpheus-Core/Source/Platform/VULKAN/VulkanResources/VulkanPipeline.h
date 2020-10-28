#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSwapchain.h"
#include "Platform/Vulkan/VulkanResources/VulkanCommand.h"

#include "Platform/Vulkan/VulkanBindables/VulkanShader.h"
#include "Platform/Vulkan/VulkanResources/VulkanDescriptorPool.h"
#include "Platform/Vulkan/VulkanUnknown/VulkanRenderpass.h"

#include "Morpheus/Renderer/RendererResources/GraphicsPipeline.h"

namespace Morpheus {

	class VulkanPipeline : public GraphicsPipeline
	{
	public:
		VulkanPipeline();
		virtual ~VulkanPipeline();
		void Destory();

		const uint32& GetID() { return m_ID; }
		void SetID(const uint32& _ID) { m_ID = _ID; }

		const vk::Pipeline& GetPipeline() { return m_Pipeline; }
		const vk::PipelineLayout& GetLayout() { return m_PipelineLayout; }

	private:
		void VertexInputData();
		void CreatePipeline();

	private:
		vk::Pipeline m_Pipeline;
		vk::PipelineCache m_PipelineCache;
		vk::PipelineLayout m_PipelineLayout;

		vk::PipelineVertexInputStateCreateInfo m_InputState;
		vk::VertexInputBindingDescription m_InputBinding;
		Vector<vk::VertexInputAttributeDescription> m_InputAttributes;

		Ref<VulkanDevice> m_Device;
		Ref<VulkanShader> m_Shader;
		Ref<VulkanSwapchain> m_Swapchain;
		Ref<VulkanRenderpass> m_Renderpass;
		Ref<VulkanDescriptorPool> m_DescriptorPool;

		uint32 m_ID = 0;

	public:
		static Ref<VulkanPipeline> VulkanCreate();
	};

}