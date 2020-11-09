#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSurface.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"

#include "Morpheus/Renderer/RendererResources/Renderpass.h"

#include "VulkanResource.h"

namespace Morpheus {

	class VulkanRenderpass : public VulkanResource, public Renderpass
	{
	public:
		VulkanRenderpass(const RenderpassLayout& _Layout);
		virtual ~VulkanRenderpass();

	private:
		virtual void VulkanCreate() override;
		virtual void VulkanDestory() override;

	public:
		const vk::RenderPass& GetRenderpass() { return m_RenderPass; }

	private:
		void InvalidateAttachments();

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanSurface> m_Surface;

		RenderpassLayout m_Layout;
		Vector<vk::AttachmentDescription> m_Attachments;
		Vector<vk::SubpassDependency> m_Dependencies;

		Vector<vk::AttachmentReference> m_ColorReference;
		Vector<vk::AttachmentReference> m_DepthReference;
		Vector<vk::SubpassDescription> m_SubpassDesc;

		vk::RenderPass m_RenderPass;

	public:
		static Ref<VulkanRenderpass> Make(const RenderpassLayout& _Layout);
	};

}