// MORHPEUS VULKAN BACKEND - VERSION 1.0 [06:09:20]
#pragma once

#include "Morpheus/Core/Common.h"
#include <vulkan/vulkan.h>

namespace Morpheus {

	class VulkanInstance
	{
	protected:
		VulkanInstance();
		~VulkanInstance();

	public:
		static VulkanInstance* GetInstance();

		void Init();
		void Shutdown();

		VkInstance GetVulkan() { return m_IVulkan; };

		// TODO: SPLIT THE VALIDATION SYSTEM INTO OWN CLASS
		bool GetValidationState() { return m_EnableValidation; }
		UINT32 GetValidationSize() { return m_ValidationLayers.size(); }
		const FLOAT8** GetValidationData() { return m_ValidationLayers.data(); }

	private:
		bool CheckValidationSupport();
		FVector8 GetRequiredExtensions(UINT32 Count, const FLOAT8** Data);

		VkResult CreateDebugMessenger(VkInstance _IVulkan, const VkDebugUtilsMessengerCreateInfoEXT* _CreateInfo,
			const VkAllocationCallbacks* _Allocator, VkDebugUtilsMessengerEXT* _DebugMessenger);
		void DestroyDebugMessenger(VkInstance _IVulkan, VkDebugUtilsMessengerEXT _DebugMessenger, 
			const VkAllocationCallbacks* _Allocator);
		void SetupDebugMessenger();
		void PopulateDebugMessenger(VkDebugUtilsMessengerCreateInfoEXT& CreateInfo);

	private: 
		static VulkanInstance* s_Instance;
		VkInstance m_IVulkan;

		bool m_EnableValidation = true;
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		FVector8 m_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };
	};

}