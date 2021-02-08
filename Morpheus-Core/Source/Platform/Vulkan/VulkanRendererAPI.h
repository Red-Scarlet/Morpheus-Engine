#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Renderer/RendererCore/RendererAPI.h"
#include "Platform/Vulkan/VulkanCommand/VulkanCommandPool.h"

#include "VulkanSystems/VulkanAllocator.h"
#include "VulkanSystems/VulkanDeallocator.h"
#include "VulkanSystems/VulkanExecutor.h"
#include "VulkanSystems/VulkanRenderer.h"
#include "VulkanSystems/VulkanUpdator.h"

namespace Morpheus { namespace Vulkan {

	class VulkanRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void Shutdown() override;

		virtual void BindGraph(const Ref<RenderGraph>& _RenderGraph) override;
		virtual void DrawIndexed(const Ref<VertexArray>& _VertexArray) override;

	private:
		Ref<VulkanCommandPool> m_Pool;
		Ref<VulkanAllocator> m_Allocator;
		Ref<VulkanDeallocator> m_Deallocator;
		Ref<VulkanExecutor> m_Executor;
		Ref<VulkanRenderer> m_Renderer;
		Ref<VulkanUpdator> m_Updator;
	};

}}