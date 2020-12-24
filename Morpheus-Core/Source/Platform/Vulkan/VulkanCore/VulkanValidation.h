#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

namespace Morpheus { namespace Vulkan {

	class VulkanValidation
	{
	public:
		VulkanValidation();
		~VulkanValidation();

		void PopulateDebugMessenger(VkDebugUtilsMessengerCreateInfoEXT& CreateInfo);
		const uint32& GetValidationSize() { return m_ValidationLayers.size(); }
		const float8** GetValidationData() { return m_ValidationLayers.data(); }

	private:
		bool CheckValidationSupport();	
		VkResult CreateDebugMessenger(const VkInstance& _IVulkan, const VkDebugUtilsMessengerCreateInfoEXT* _CreateInfo,
			const VkAllocationCallbacks* _Allocator, VkDebugUtilsMessengerEXT* _DebugMessenger);
		void DestroyDebugMessenger(const VkInstance& _IVulkan, VkDebugUtilsMessengerEXT _DebugMessenger,
			const VkAllocationCallbacks* _Allocator);
		void SetupDebugMessenger();

	private:
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		Vector<const float8*> m_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };

	public:
		static Ref<VulkanValidation> Create();

	};

}}
