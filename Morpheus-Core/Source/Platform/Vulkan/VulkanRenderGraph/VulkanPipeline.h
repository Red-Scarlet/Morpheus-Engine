#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"
#include "Platform/Vulkan/VulkanRenderGraph/VulkanDescriptor.h"

#include "Morpheus/Renderer/RendererComponents/RendererRenderPipelineInfo.h"

namespace Morpheus { namespace Vulkan {

	struct VulkanPipelineInfo
	{
		RenderPipelineStruct Struct;
		VkRenderPass RenderPass;
		uint32 ColorAttachments;
		uint32 DepthAttachments;
		uint32 SubpassIndex;
	};

	class VulkanPipeline
	{
	public:
		VulkanPipeline(const VulkanPipelineInfo& _PipelineInfo);
		virtual ~VulkanPipeline();

		const VkPipeline& GetPipeline() { return m_Pipeline; }
		const VkPipelineLayout& GetPipelineLayout() { return m_PipelineLayout; }

	private:
		void VulkanCreate();
		void VulkanDestory();

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanDescriptor> m_Descriptor;
		VulkanPipelineInfo m_Struct;

		VkPipeline m_Pipeline;
		VkPipelineCache m_PipelineCache;
		VkPipelineLayout m_PipelineLayout;

	public:
		static Ref<VulkanPipeline> Create(const VulkanPipelineInfo& _PipelineInfo);
		static void Destroy(const Ref<VulkanPipeline>& _Pipeline);
	};

}}