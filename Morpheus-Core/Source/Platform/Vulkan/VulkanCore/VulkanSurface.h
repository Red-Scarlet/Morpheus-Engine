#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

namespace Morpheus { namespace Vulkan {

	class VulkanSurface
	{
	public:
		VulkanSurface(const VkInstance& _Instance);
		virtual ~VulkanSurface();

		const VkSurfaceKHR& GetSurface() const { return m_Surface; };

	private:
		void CreateSurface();
		void DestroySurface();

	private:
		VkInstance m_Instance;
		VkSurfaceKHR m_Surface;

	public:
		static Ref<VulkanSurface> Create(const VkInstance& _Instance);
		static void Destroy(const Ref<VulkanSurface>& _Surface);
	};

}}