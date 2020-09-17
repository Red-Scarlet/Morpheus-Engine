#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Renderer/RendererAPI.h"

#include "Platform/Vulkan/VulkanCore/VulkanInstance.h"
#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"
#include "Platform/Vulkan/VulkanCore/VulkanPresentation.h"

#include "Platform/Vulkan/VulkanCommand/VulkanCommandSystem.h"
#include "Platform/Vulkan/VulkanCommand/VulkanSynchronization.h"

namespace Morpheus {

	class VulkanRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void Shutdown() override;

		virtual void Begin() override;
		virtual void End() override;
		virtual void Flush() override;

		virtual void Reset() override;

		virtual void SetViewport() override;
		virtual void SetClearColor(const Vector4& _Color) override;
		virtual void SetRenderpass(const Ref<Renderpass>& _Renderpass) override;
		virtual void SetPipeline(const Ref<Pipeline>& _Pipeline) override;
		virtual void SetFramebuffer(const Ref<Framebuffer>& _Framebuffer) override;
		virtual void DrawGeomerty() override;

	private:
		VulkanInstance* m_VulkanInstance;
		VulkanPresentation* m_VulkanPresentation;
		VulkanLogicalDevice* m_VulkanDevice;
		
		// COMBINE INTO COMMAND SYSTEM
		VulkanCommandSystem* m_VulkanCommandSystem;
		VulkanCommandBuffer* m_CommandBuffer;

		VulkanSynchronization* m_VulkanSynchronization;
	};

}