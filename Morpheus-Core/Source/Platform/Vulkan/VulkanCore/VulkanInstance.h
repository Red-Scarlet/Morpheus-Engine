// MORHPEUS VULKAN BACKEND - VERSION 1.3 [130920]
#pragma once

#include "Morpheus/Core/Common.h"

#include "VulkanValidation.h"
#include "VulkanPresentation.h"
#include "VulkanDevice.h"

#include <vulkan/vulkan.h>

namespace Morpheus {

	class VulkanInstance
	{
	public:
		friend class VulkanValidation;
		friend class VulkanPresentation;
		friend class VulkanPhysicalDevice;
		friend class VulkanLogicalDevice;

	protected:
		VulkanInstance();
		~VulkanInstance();

	public:
		static VulkanInstance* GetInstance();
		void Shutdown();
		void Init();

	public:
		VulkanPresentation* GetPresentation();
		VulkanPhysicalDevice* GetPhysicalDevice();
		VulkanLogicalDevice* GetLogicalDevice();

		const VkInstance& GetVulkan() { return m_VulkanObject.Vulkan; }
		const bool& GetDebug() { return m_VulkanDebug.ValidationState; }
	private:
		Vector<const float8*> GetRequiredExtensions(uint32 Count, const float8** Data);

	private:
		static VulkanInstance* s_Instance;

		struct {
			VkInstance Vulkan = nullptr;
			VulkanPresentation* Presentation = nullptr;
			VulkanPhysicalDevice* PhysicalDevice = nullptr;
			VulkanLogicalDevice* LogicalDevice = nullptr;
		} m_VulkanObject;

		struct {
			VulkanValidation* ValidationLayer = nullptr;
			bool ValidationState = true;
		} m_VulkanDebug;

	};

}