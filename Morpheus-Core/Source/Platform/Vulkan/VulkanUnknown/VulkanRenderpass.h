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
		VulkanRenderpass();
		virtual ~VulkanRenderpass();
		void Destory();

		const uint32& GetID() { return m_ID; }
		void SetID(const uint32& _ID) { m_ID = _ID; }

		const vk::RenderPass& GetRenderpass() { return m_RenderPass; }

	private:
		void CreateRenderpass();

	private:
		vk::RenderPass m_RenderPass;
		Ref<VulkanDevice> m_Device;

		uint32 m_ID = 0;

	public:
		static Ref<VulkanRenderpass> VulkanCreate();
	};

}