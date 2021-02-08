#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"
#include "Platform/Vulkan/VulkanComponents/VulkanMemoryInfo.h"

#include "Morpheus/Renderer/RendererPipeline/RenderGraph.h"
#include "Platform/Vulkan/VulkanRenderGraph/VulkanRenderBuffer.h"
#include "Platform/Vulkan/VulkanRenderGraph/VulkanPipeline.h"

namespace Morpheus { namespace Vulkan {

	// This will make CommandBuffers, Attach to resource.
	class VulkanRenderGraph : public IVulkanResource
	{
	public:
		VulkanRenderGraph(const AnyData& _Data, const Resource& _Resource);
		virtual ~VulkanRenderGraph();

	private:
		void VulkanUpdate();
		void VulkanDestory();

		void BuildRenderBuffers();
		void BuildRenderPass();
		void BuildFrameBuffer();
		void BuildPipelines();

	private:
		Ref<VulkanDevice> m_Device;
		RenderGraphLayout m_Layout;
		Resource m_Resource;

		// TODO MAKE POOL ALLOCATED!
		Vector<Ref<VulkanRenderBuffer>> m_RenderBuffers;
		VkRenderPass m_RenderPass;
		bool m_RenderPassCompiled;
		
		VkFramebuffer m_FrameBuffer;
		bool m_FrameBufferCompiled;

		Vector<Ref<VulkanPipeline>> m_Pipelines;


	public:
		static Ref<VulkanRenderGraph> Create(const AnyData& _Data, const Resource& _Resource);
		static void Destroy(const Ref<VulkanRenderGraph>& _RenderGraph);
		static void Update(const Ref<VulkanRenderGraph>& _RenderGraph);
	};

}}