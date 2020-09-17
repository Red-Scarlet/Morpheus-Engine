// MORHPEUS VULKAN BACKEND - VERSION 1.3 [130920]
#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Renderer/RendererCore/Renderpass.h"
#include "Morpheus/Renderer/RendererCore/Pipeline.h"

#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"
#include "Platform/Vulkan/VulkanCore/VulkanPresentation.h"

#include <vulkan/vulkan.h>

namespace Morpheus {

	//TODO SPLIT SHADER UP INTO OWNING CLASS
	class VulkanGraphicsPipeline : public Pipeline
	{
	public:
		VulkanGraphicsPipeline(const Ref<Renderpass>& _Renderpass);
		~VulkanGraphicsPipeline();

		VkPipeline GetPipeline() { return m_VulkanObject.Pipeline; }

	private:
		void CreateGraphicsPipeline();
		VkShaderModule CreateShaderModule(const Vector<float8>& Code);

	private:
		struct {
			VulkanLogicalDevice* lDevice;
			VulkanPresentation* Presentation;
			Ref<Renderpass> Renderpass;
		} m_VulkanCore;

		struct {
			VkPipelineLayout PipelineLayout;
			VkPipeline Pipeline;			

		} m_VulkanObject;

	};


}