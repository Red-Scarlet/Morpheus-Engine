// MORHPEUS VULKAN BACKEND - VERSION 1.3 [130920]
#pragma once

#include "Morpheus/Core/Common.h"

#include <vulkan/vulkan.h>

namespace Morpheus {

	class VulkanValidation
	{
	public:
		VulkanValidation();
		~VulkanValidation();

		void PopulateDebugMessenger(VkDebugUtilsMessengerCreateInfoEXT& CreateInfo);
		uint32 GetValidationSize() { return m_ValidationLayers.size(); }
		const float8** GetValidationData() { return m_ValidationLayers.data(); }

	private:
		bool CheckValidationSupport();	
		VkResult CreateDebugMessenger(VkInstance _IVulkan, const VkDebugUtilsMessengerCreateInfoEXT* _CreateInfo,
			const VkAllocationCallbacks* _Allocator, VkDebugUtilsMessengerEXT* _DebugMessenger);
		void DestroyDebugMessenger(VkInstance _IVulkan, VkDebugUtilsMessengerEXT _DebugMessenger,
			const VkAllocationCallbacks* _Allocator);
		void SetupDebugMessenger();

	private:
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		Vector<const float8*> m_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };

	};

}
