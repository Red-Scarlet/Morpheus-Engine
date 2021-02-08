#pragma once

#include <vulkan/vulkan.h>
#include "Morpheus/Mathematics/Mathematics.h"
#include "Morpheus/Renderer/RendererTypes.h"

#define VULKAN_CORE_LOGGING
#define VULKAN_CORE_LOG_EXT

#define VULKAN_CORE_LOG_POINTERS
//#define VULKAN_CORE_LOG_COMMANDS
//#define VULKAN_CORE_LOG_BINDINGS

#define VULKAN_ERROR true

#ifdef MORP_CORE_ASSERTS
	#define VULKAN_CORE_ASSERT(x, ...) { MORP_CORE_ASSERT(!x, __VA_ARGS__); }
#else
	#define VULKAN_CORE_ASSERT(x, ...)
#endif

#ifdef VULKAN_CORE_LOGGING
	#define VULKAN_CORE_TRACE(...)					MORP_CORE_TRACE(__VA_ARGS__)
	#define VULKAN_CORE_INFO(...)					MORP_CORE_INFO(__VA_ARGS__)
	#define VULKAN_CORE_WARN(...)					MORP_CORE_WARN(__VA_ARGS__)
	#define VULKAN_CORE_ERROR(...)					MORP_CORE_ERROR(__VA_ARGS__)
	#define VULKAN_CORE_SPECIAL(...)				MORP_CORE_SPECIAL(__VA_ARGS__)
	#define VULKAN_CORE_SPECIAL_2(...)				MORP_CORE_SPECIAL_2(__VA_ARGS__)

	#ifdef VULKAN_CORE_LOG_POINTERS
		#define VULKAN_CORE_DEBUG_PTR(...)			VULKAN_CORE_SPECIAL("[VULKAN] Memory Address: 0x" + Morpheus::ReadAddress(__VA_ARGS__))
	#else
		#define VULKAN_CORE_DEBUG_PTR(...)
	#endif

	#ifdef VULKAN_CORE_LOG_COMMANDS
		#define VULKAN_CORE_DEBUG_CMD(VAR, VAR_2)	VULKAN_CORE_SPECIAL("[VULKAN] Command Call: " + Morpheus::GetString(VAR) + " " + Morpheus::GetString(VAR_2))
	#else
		#define VULKAN_CORE_DEBUG_CMD(VAR, VAR_2)
	#endif

	#ifdef VULKAN_CORE_LOG_BINDINGS
		#define VULKAN_CORE_DEBUG_BOUND(VAR)		VULKAN_CORE_SPECIAL("[VULKAN] Bounded ID: " + Morpheus::GetString(VAR))
	#else
		#define VULKAN_CORE_DEBUG_BOUND(VAR)
	#endif
	
#else
	#define VULKAN_CORE_TRACE(...)
	#define VULKAN_CORE_INFO(...)
	#define VULKAN_CORE_WARN(...)
	#define VULKAN_CORE_ERROR(...)
	#define VULKAN_CORE_SPECIAL(...)
	#define VULKAN_CORE_SPECIAL_2(...)
	#define VULKAN_CORE_DEBUG_PTR(...)
	#define VULKAN_CORE_DEBUG_CMD(VAR, VAR_2)
	#define VULKAN_CORE_DEBUG_BOUND(VAR)
#endif

namespace Morpheus { namespace Vulkan {


	static VkFormat GetVulkanFormatFromEngine(const RendererImageFormat& _Format)
	{
		switch (_Format)
		{
			case RendererImageFormat::FORMAT_DEFAULT:					return VkFormat::VK_FORMAT_UNDEFINED;
				 
			case RendererImageFormat::FORMAT_R8G8B8_SINT:				return VkFormat::VK_FORMAT_R8G8B8_SINT;
			case RendererImageFormat::FORMAT_R16G16B16_SINT:			return VkFormat::VK_FORMAT_R16G16B16_SINT;
			case RendererImageFormat::FORMAT_R32G32B32_SINT:			return VkFormat::VK_FORMAT_R32G32B32_SINT;
			case RendererImageFormat::FORMAT_R8G8B8A8_SINT:				return VkFormat::VK_FORMAT_R8G8B8A8_SINT;
			case RendererImageFormat::FORMAT_R16G16B16A16_SINT:			return VkFormat::VK_FORMAT_R16G16B16A16_SINT;
			case RendererImageFormat::FORMAT_R32G32B32A32_SINT:			return VkFormat::VK_FORMAT_R32G32B32A32_SINT;
				 
			case RendererImageFormat::FORMAT_D32_SFLOAT:				return VkFormat::VK_FORMAT_D32_SFLOAT;
			case RendererImageFormat::FORMAT_R16G16B16_SFLOAT:			return VkFormat::VK_FORMAT_R16G16B16_SFLOAT;
			case RendererImageFormat::FORMAT_R32G32B32_SFLOAT:			return VkFormat::VK_FORMAT_R32G32B32_SFLOAT;
			case RendererImageFormat::FORMAT_R16G16B16A16_SFLOAT:		return VkFormat::VK_FORMAT_R16G16B16A16_SFLOAT;
			case RendererImageFormat::FORMAT_R32G32B32A32_SFLOAT:		return VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT;
				 
			case RendererImageFormat::FORMAT_R8G8B8_UNORM:				return VkFormat::VK_FORMAT_R8G8B8_UNORM;
			case RendererImageFormat::FORMAT_R16G16B16_UNORM:			return VkFormat::VK_FORMAT_R16G16B16_UNORM;
			case RendererImageFormat::FORMAT_R8G8B8A8_UNORM:			return VkFormat::VK_FORMAT_R8G8B8A8_UNORM;
			case RendererImageFormat::FORMAT_R16G16B16A16_UNORM:		return VkFormat::VK_FORMAT_R16G16B16A16_UNORM;	 
				 
			case RendererImageFormat::FORMAT_R8G8B8_SNORM:				return VkFormat::VK_FORMAT_R8G8B8_SNORM;
			case RendererImageFormat::FORMAT_R16G16B16_SNORM:			return VkFormat::VK_FORMAT_R16G16B16_SNORM;
			case RendererImageFormat::FORMAT_R8G8B8A8_SNORM:			return VkFormat::VK_FORMAT_R8G8B8A8_SNORM;
			case RendererImageFormat::FORMAT_R16G16B16A16_SNORM:		return VkFormat::VK_FORMAT_R16G16B16A16_SNORM;
		}

		MORP_CORE_ASSERT(MORP_ERROR, "Unknown RendererImageFormat!");
		return VkFormat::VK_FORMAT_UNDEFINED;
	}

	
	static VkImageLayout GetVulkanLayoutFromEngine(const RendererImageLayout& _Layout)
	{
		switch (_Layout)
		{
			case RendererImageLayout::USAGE_UNDEFINED_ATTACHMENT:			return VK_IMAGE_LAYOUT_UNDEFINED;
			case RendererImageLayout::USAGE_PRESENT_ATTACHMENT:				return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			case RendererImageLayout::USAGE_COLOR_ATTACHMENT:				return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			case RendererImageLayout::USAGE_DEPTH_STENCIL_ATTACHMENT:		return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

			case RendererImageLayout::USAGE_SHADER_READ_ATTACHMENT:			return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			case RendererImageLayout::USAGE_DEPTH_STENCIL_READ_ATTACHMENT:	return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
		}

		MORP_CORE_ASSERT(MORP_ERROR, "Unknown RendererImageUsage!");
		return VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;
	}

	static VkImageUsageFlagBits GetVulkanUsageFromEngine(const RendererImageUsage& _Usage)
	{
		switch (_Usage)
		{
			case RendererImageUsage::USAGE_COLOR_BIT:					return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			case RendererImageUsage::USAGE_DEPTH_STENCIL_BIT:			return VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		}

		MORP_CORE_ASSERT(MORP_ERROR, "Unknown RendererImageUsage!");
		return VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	}

	static const VkFormat& GetVulkanShaderAttributeFromEngine(const PipelineAttributeType& _Type)
	{
		switch (_Type)
		{
			case PipelineAttributeType::Float:			return VK_FORMAT_R32_SFLOAT;
			case PipelineAttributeType::Float2:			return VK_FORMAT_R32G32_SFLOAT;
			case PipelineAttributeType::Float3:			return VK_FORMAT_R32G32B32_SFLOAT;
			case PipelineAttributeType::Float4:			return VK_FORMAT_R32G32B32A32_SFLOAT;
			case PipelineAttributeType::Mat3:			return VK_FORMAT_R32G32B32A32_SFLOAT;
			case PipelineAttributeType::Mat4:			return VK_FORMAT_R32G32B32A32_SFLOAT;
			case PipelineAttributeType::Int:			return VK_FORMAT_R32_SINT;
			case PipelineAttributeType::Int2:			return VK_FORMAT_R32G32_SINT;
			case PipelineAttributeType::Int3:			return VK_FORMAT_R32G32B32_SINT;
			case PipelineAttributeType::Int4:			return VK_FORMAT_R32G32B32A32_SINT;
			case PipelineAttributeType::Bool:			return VK_FORMAT_R32_SINT;
		}
		MORP_CORE_ASSERT(MORP_ERROR, "Unknown PipelineAttributeType!");
		return VK_FORMAT_UNDEFINED;
	}

	static const VkDescriptorType& GetVulkanDescriptorTypeFromEngine(const PipelineDescriptorType& _Type)
	{
		switch (_Type)
		{
			case PipelineDescriptorType::DESCRIPTOR_TYPE_SAMPLER:							return VK_DESCRIPTOR_TYPE_SAMPLER;
			case PipelineDescriptorType::DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:			return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			case PipelineDescriptorType::DESCRIPTOR_TYPE_SAMPLED_IMAGE:						return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
			case PipelineDescriptorType::DESCRIPTOR_TYPE_STORAGE_IMAGE:						return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
			case PipelineDescriptorType::DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER:				return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
			case PipelineDescriptorType::DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER:				return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
			case PipelineDescriptorType::DESCRIPTOR_TYPE_UNIFORM_BUFFER:					return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			case PipelineDescriptorType::DESCRIPTOR_TYPE_STORAGE_BUFFER:					return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
			case PipelineDescriptorType::DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC:			return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
			case PipelineDescriptorType::DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC:			return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
			case PipelineDescriptorType::DESCRIPTOR_TYPE_INPUT_ATTACHMENT:					return VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
		}
		MORP_CORE_ASSERT(MORP_ERROR, "Unknown PipelineDescriptorType!");
		return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	}

	static const VkShaderStageFlagBits& GetVulkanDescriptorStageFromEngine(const PipelineDescriptorStage& _Type)
	{
		switch (_Type)
		{
			case PipelineDescriptorStage::DESCRIPTOR_VERTEX:			return VK_SHADER_STAGE_VERTEX_BIT;
			case PipelineDescriptorStage::DESCRIPTOR_FRAGMENT:		return VK_SHADER_STAGE_FRAGMENT_BIT;
		}
		MORP_CORE_ASSERT(MORP_ERROR, "Unknown PipelineDescriptorStage!");
		return VK_SHADER_STAGE_ALL_GRAPHICS;
	}

}}