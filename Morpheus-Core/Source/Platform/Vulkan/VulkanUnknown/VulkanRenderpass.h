#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSurface.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"

#include "Morpheus/Renderer/RendererUnknown/Renderpass.h"

namespace Morpheus {

	class VulkanRenderpass : public Renderpass
	{
	public:
		VulkanRenderpass(const RenderpassLayout& _Layout);
		virtual ~VulkanRenderpass();
		void Destory();

		virtual const uint32& GetID() override 
		{ return m_ID; }
		void SetID(const uint32& _ID) { m_ID = _ID; }

		const vk::RenderPass& GetRenderpass() { return m_RenderPass; }

		void InvalidateAttachments();

	private:
		void CreateRenderpass();

	private:
		Ref<VulkanDevice> m_Device;

		RenderpassLayout m_Layout;
		Vector<vk::AttachmentDescription> m_Attachments;
		Vector<vk::SubpassDependency> m_Dependencies;

		Vector<vk::AttachmentReference> m_ColorReference;
		Vector<vk::AttachmentReference> m_DepthReference;
		Vector<vk::SubpassDescription> m_SubpassDesc;

		vk::RenderPass m_RenderPass;
		uint32 m_ID = 0;

	public:
		static Ref<VulkanRenderpass> VulkanCreate(const RenderpassLayout& _Layout);
	};

}