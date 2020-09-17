// MORHPEUS VULKAN BACKEND - VERSION 1.3 [130920]
#include "Morppch.h"
#include "VulkanGraphicsPipeline.h"

#include "Platform/Vulkan/VulkanCore/VulkanInstance.h"
#include "Platform/Vulkan/VulkanGraphics/VulkanRenderpass.h"

#include <fstream>

namespace Morpheus {

	static Vector<float8> ReadFile(const String& Filename)
	{
		std::ifstream file(Filename, std::ios::ate | std::ios::binary);

		if (!file.is_open())
			MORP_CORE_ASSERT(true, "Failed to open file!");
		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);
		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}

	VulkanGraphicsPipeline::VulkanGraphicsPipeline(const Ref<Renderpass>& _Renderpass)
	{
		auto Instance = VulkanInstance::GetInstance();
		m_VulkanCore.lDevice = Instance->GetLogicalDevice();
		m_VulkanCore.Presentation = Instance->GetPresentation();
		m_VulkanCore.Renderpass = _Renderpass;

		CreateGraphicsPipeline();
		MORP_CORE_WARN("[VULKAN] GraphicsPipeline Was Created!");
	}

	VulkanGraphicsPipeline::~VulkanGraphicsPipeline()
	{
		vkDestroyPipeline(m_VulkanCore.lDevice->GetDevice(), m_VulkanObject.Pipeline, nullptr);
		vkDestroyPipelineLayout(m_VulkanCore.lDevice->GetDevice(), m_VulkanObject.PipelineLayout, nullptr);
	}

	void VulkanGraphicsPipeline::CreateGraphicsPipeline()
	{
		auto vertShaderCode = ReadFile("Assets//vert.spv");
		auto fragShaderCode = ReadFile("Assets//frag.spv");

		VkShaderModule vertShaderModule = CreateShaderModule(vertShaderCode);
		VkShaderModule fragShaderModule = CreateShaderModule(fragShaderCode);

		VkPipelineShaderStageCreateInfo vertShaderStageInfo {};
		{
			vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
			vertShaderStageInfo.module = vertShaderModule;
			vertShaderStageInfo.pName = "main";
		}

		VkPipelineShaderStageCreateInfo fragShaderStageInfo {};
		{
			fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			fragShaderStageInfo.module = fragShaderModule;
			fragShaderStageInfo.pName = "main";
		}

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

		VkPipelineVertexInputStateCreateInfo vertexInputInfo {};
		{
			vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			vertexInputInfo.vertexBindingDescriptionCount = 0;
			vertexInputInfo.vertexAttributeDescriptionCount = 0;
		}

		VkPipelineInputAssemblyStateCreateInfo inputAssembly {};
		{
			inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			inputAssembly.primitiveRestartEnable = VK_FALSE;
		}

		VkViewport viewport {};
		{
			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = (float32)m_VulkanCore.Presentation->GetExtent().width;
			viewport.height = (float32)m_VulkanCore.Presentation->GetExtent().height;
			viewport.minDepth = 0.0f;
			viewport.maxDepth = 1.0f;
		}

		VkRect2D scissor {};
		{
			scissor.offset = { 0, 0 };
			scissor.extent = m_VulkanCore.Presentation->GetExtent();
		}

		VkPipelineViewportStateCreateInfo viewportState {};
		{
			viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewportState.viewportCount = 1;
			viewportState.pViewports = &viewport;
			viewportState.scissorCount = 1;
			viewportState.pScissors = &scissor;
		}

		VkPipelineRasterizationStateCreateInfo rasterizer {};
		{
			rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			rasterizer.depthClampEnable = VK_FALSE;
			rasterizer.rasterizerDiscardEnable = VK_FALSE;
			rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
			rasterizer.lineWidth = 1.0f;
			rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
			rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
			rasterizer.depthBiasEnable = VK_FALSE;
		}

		VkPipelineMultisampleStateCreateInfo multisampling {};
		{
			multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			multisampling.sampleShadingEnable = VK_FALSE;
			multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		}

		VkPipelineColorBlendAttachmentState colorBlendAttachment {};
		{
			colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			colorBlendAttachment.blendEnable = VK_FALSE;
		}
	
		VkPipelineColorBlendStateCreateInfo colorBlending {};
		{
			colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			colorBlending.logicOpEnable = VK_FALSE;
			colorBlending.logicOp = VK_LOGIC_OP_COPY;
			colorBlending.attachmentCount = 1;
			colorBlending.pAttachments = &colorBlendAttachment;
			colorBlending.blendConstants[0] = 0.0f;
			colorBlending.blendConstants[1] = 0.0f;
			colorBlending.blendConstants[2] = 0.0f;
			colorBlending.blendConstants[3] = 0.0f;
		}

		VkPipelineLayoutCreateInfo pipelineLayoutInfo {};
		{
			pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipelineLayoutInfo.setLayoutCount = 0;
			pipelineLayoutInfo.pushConstantRangeCount = 0;
		}

		if (vkCreatePipelineLayout(m_VulkanCore.lDevice->GetDevice(), &pipelineLayoutInfo, nullptr, &m_VulkanObject.PipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout!");
		}

		VkGraphicsPipelineCreateInfo pipelineInfo {};
		{
			pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			pipelineInfo.stageCount = 2;
			pipelineInfo.pStages = shaderStages;
			pipelineInfo.pVertexInputState = &vertexInputInfo;
			pipelineInfo.pInputAssemblyState = &inputAssembly;
			pipelineInfo.pViewportState = &viewportState;
			pipelineInfo.pRasterizationState = &rasterizer;
			pipelineInfo.pMultisampleState = &multisampling;
			pipelineInfo.pColorBlendState = &colorBlending;
			pipelineInfo.layout = m_VulkanObject.PipelineLayout;
			pipelineInfo.renderPass = CastRef<VulkanRenderpass>(m_VulkanCore.Renderpass)->GetRenderpass();
			pipelineInfo.subpass = 0;
			pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
		}

		if (vkCreateGraphicsPipelines(m_VulkanCore.lDevice->GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_VulkanObject.Pipeline) != VK_SUCCESS) {
			throw std::runtime_error("failed to create graphics pipeline!");
		}

		vkDestroyShaderModule(m_VulkanCore.lDevice->GetDevice(), fragShaderModule, nullptr);
		vkDestroyShaderModule(m_VulkanCore.lDevice->GetDevice(), vertShaderModule, nullptr);
	}

	VkShaderModule VulkanGraphicsPipeline::CreateShaderModule(const Vector<float8>& Code)
	{
		VkShaderModuleCreateInfo createInfo {};
		{
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = Code.size();
			createInfo.pCode = reinterpret_cast<const UINT32*>(Code.data());
		}

		VkShaderModule shaderModule;
		VkResult result = vkCreateShaderModule(m_VulkanCore.lDevice->GetDevice(), &createInfo, nullptr, &shaderModule);
		MORP_CORE_ASSERT(result, "Failed to create Shader Module!");
		return shaderModule;
	}

}
