#pragma once
#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanCache.h"
#include "Platform/Vulkan/VulkanCore/VulkanQueue.h"
#include "Platform/Vulkan/VulkanCore/VulkanSwapchain.h"

namespace Morpheus { namespace Vulkan {

	#define VULKAN_CACHE_DEVICE_TYPE VulkanCacheType(0)
	class VulkanDevice
	{
	public:
		typedef VulkanCacheMember<VulkanDevice> DeviceCache;

	public:
		VulkanDevice(const VkInstance& _Instance, const VkSurfaceKHR& _Surface);
		virtual ~VulkanDevice();

		void CreateSwapchain();
		uint32 FindMemoryType(const uint32& _TypeFilter, const VkMemoryPropertyFlags& _Properties);
		VkFormat FindFormatType(const Vector<VkFormat>& _Formats, const VkImageTiling& _Tiling, const VkFormatFeatureFlags& _Features);
		const VkDevice& GetLogical() { MORP_PROFILE_FUNCTION(); return m_lDevice; };
		const uint32& GetQueueIndices(const QueueType& _QueueType);
		const Ref<VulkanQueue> GetQueue(const QueueType& _QueueType);

	private:
		void PickPhysicalDevice();
		bool IsDeviceSuitable(const VkPhysicalDevice& _Device);
		void MakeLogicalDevice();
		void DestroyLogicalDevice();

	private:
		VkInstance m_Instance;
		VkSurfaceKHR m_Surface;

		VkPhysicalDevice m_pDevice;
		VkDevice m_lDevice;

		QueueFamilyIndices m_Indices;
		SwapchainSupportDetails m_SwapchainSupport;
		Vector<const float8*> m_Extensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

		Map<QueueType, Ref<VulkanQueue>> m_Queues;
		Ref<VulkanSwapchain> m_Swapchain;

	public:
		static Ref<VulkanDevice> Create(const VkInstance& _Instance, const VkSurfaceKHR& _Surface);
		static void Destroy(const Ref<VulkanDevice>& _Device);
	};

}}