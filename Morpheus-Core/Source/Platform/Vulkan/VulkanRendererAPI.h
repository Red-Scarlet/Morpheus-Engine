#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Renderer/RendererAPI.h"

#include "Platform/Vulkan/VulkanCore/VulkanInstance.h"
#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"
#include "Platform/Vulkan/VulkanCore/VulkanPresentation.h"

#include "Platform/Vulkan/VulkanGraphics/VulkanGraphicsPipeline.h"
#include "Platform/Vulkan/VulkanGraphics/VulkanRenderpass.h"
#include "Platform/Vulkan/VulkanGraphics/VulkanFramebuffer.h"

#include "Platform/Vulkan/VulkanGraphics/VulkanCommandSystem.h"
#include "Platform/Vulkan/VulkanGraphics/VulkanSynchronization.h"

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

	private:
		VulkanInstance* m_VulkanInstance;
		VulkanPresentation* m_VulkanPresentation;
		VulkanLogicalDevice* m_VulkanDevice;

		// MAKE REAL OBJECTS INSIDE ENGINE SIDE.
		// GRAB THE SHADERS.
		VulkanRenderpass* m_VulkanRenderpass;
		VulkanGraphicsPipeline* m_VulkanGraphicsPipeline;
		VulkanFramebuffer* m_VulkanFramebuffer;
		
		// COMBINE INTO COMMAND SYSTEM
		VulkanCommandSystem* m_VulkanCommandSystem;
		VulkanCommandBuffer* m_CommandBuffer;

		VulkanSynchronization* m_VulkanSynchronization;
	};

}