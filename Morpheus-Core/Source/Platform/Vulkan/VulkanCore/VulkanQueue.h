#pragma once
#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

namespace Morpheus { namespace Vulkan {

	enum class QueueType : uint8
	{ VULKAN_QUEUE_GRAPHICS, VULKAN_QUEUE_PRESENT, VULKAN_QUEUE_COMPUTE, VULKAN_QUEUE_TRANSFER };

	struct QueueFamilyIndices
	{
	public:
		Optional<uint32> GraphicsFamily;
		Optional<uint32> PresentFamily;
		Optional<uint32> TransferFamily;
		Optional<uint32> ComputeFamily;

		bool IsComplete()
		{ return GraphicsFamily.has_value() && PresentFamily.has_value() && TransferFamily.has_value(); }
	};

	class VulkanQueue
	{
	public:
		VulkanQueue(const VkDevice& _Device, const uint32& _QueueIndex, const uint32& _QueueFamily);
		~VulkanQueue();

		const VkQueue& GetQueue() const;
		void Submit(const VkSubmitInfo* _SubmitInfo, const VkFence& _Fence, const uint32& _Count = 1);
		void Present(const VkPresentInfoKHR* _PresentInto);
		void Wait();

	private:
		void CreateQueue();

	private:
		VkDevice m_Device;
		VkQueue m_Queue;
		uint32 m_QueueIndex;
		uint32 m_QueueFamily;

		std::mutex m_LockMutex;

	public:
		static Ref<VulkanQueue> Create(const VkDevice& _Device, const uint32& _QueueIndex, const uint32& _QueueFamily);
	};
	
}}