#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"
#include "Platform/Vulkan/VulkanCore/VulkanPresentation.h"
#include "Platform/Vulkan/VulkanCommand/VulkanCommandSystem.h"
#include "Platform/Vulkan/VulkanGraphics/VulkanRenderpass.h"

#include <vulkan/vulkan.h>

namespace Morpheus {

	class VulkanSynchronization
	{
	public:
		VulkanSynchronization(VulkanLogicalDevice* _lDevice, VulkanPresentation* _Presentation);
		~VulkanSynchronization();

		VkSemaphore* GetWait(const uint32& _Index);
		VkSemaphore* GetSignal(const uint32& _Index);

		void Flush(VulkanCommandSystem* _CommandSystem);

	private:
		void CreateSynchronization();

	private:
		struct {
			VulkanLogicalDevice* lDevice;
			VulkanPresentation* Presentation;
			VulkanCommandSystem* Command;
		} m_VulkanCore;

		struct {
			Vector<VkSemaphore> ImageSemaphores;
			Vector<VkSemaphore> RenderSemaphores;
			Vector<VkFence> InFlightFences;
			Vector<VkFence> ImagesInFlight;

			uint32 CurrentFrame = 0;
			uint32 ImageIndex = 0;

		} m_VulkanObject;

		const uint32 m_TotalNumberOfFrames = 2;

	};

}