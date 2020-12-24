#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"
#include "Platform/Vulkan/VulkanCore/VulkanSurface.h"

#ifdef MORP_DEBUG
#include "Platform/Vulkan/VulkanCore/VulkanValidation.h"
#endif

namespace Morpheus { namespace Vulkan {

	class VulkanInstance
	{
	public:
		VulkanInstance();
		~VulkanInstance();

	public:
		static Ref<VulkanInstance> GetInstance();
		void Shutdown();
		void Init();

	public:
		const VkInstance& GetVulkanInstance() { return m_VulkanInstance; }
		Ref<VulkanSurface> GetSurface();
		Ref<VulkanDevice> GetDevice(const uint32& _DeviceID);

		#ifdef MORP_DEBUG
		const Ref<VulkanValidation>& GetValidationLayer();
		#endif

	private:
		Vector<const float8*> GetRequiredExtensions();

	private:
		static Ref<VulkanInstance> s_Instance;

		VkInstance m_VulkanInstance;
		Ref<VulkanSurface> m_Surface;

		#ifdef MORP_DEBUG
		Ref<VulkanValidation> m_ValidationLayer;
		#endif

	public:
		static Ref<VulkanInstance> Create();
	};

}}