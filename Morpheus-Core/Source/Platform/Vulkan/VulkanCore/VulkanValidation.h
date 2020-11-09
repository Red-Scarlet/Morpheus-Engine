// MORHPEUS VULKAN BACKEND - VERSION 1.1 [08:09:20]
#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include <vulkan/vulkan.h>

namespace Morpheus {

	typedef const float8* VulkanByte;

	class VulkanValidation
	{
	public:
		VulkanValidation();
		~VulkanValidation();

		void PopulateDebugMessenger(VkDebugUtilsMessengerCreateInfoEXT& CreateInfo);
		uint32 GetValidationSize() { return m_ValidationLayers.size(); }
		VulkanByte* GetValidationData() { return m_ValidationLayers.data(); }

	private:
		void CreateValidationLayer();
		bool CheckValidationSupport();
		VkResult CreateDebugMessenger(VkInstance _IVulkan, const VkDebugUtilsMessengerCreateInfoEXT* _CreateInfo,
			const VkAllocationCallbacks* _Allocator, VkDebugUtilsMessengerEXT* _DebugMessenger);
		void DestroyDebugMessenger(VkInstance _IVulkan, VkDebugUtilsMessengerEXT _DebugMessenger,
			const VkAllocationCallbacks* _Allocator);
		void SetupDebugMessenger();

	private:
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		Vector<VulkanByte> m_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };

	};

}
