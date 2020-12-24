#include "Morppch.h"
#include "VulkanVertexBuffer.h"

#include "Platform/Vulkan/VulkanCore/VulkanCommand.h"
#include "Platform/Vulkan/VulkanCore/VulkanUnit.h"

namespace Morpheus { namespace Vulkan {

	VulkanVertexBuffer::VulkanVertexBuffer(QuadVertex* _Data, const uint32& _Size)
		: m_Data(_Data), m_Size(_Size)
	{
		VulkanCreate();
		VULKAN_CORE_WARN("[VULKAN] VertexBuffer Was Created!");
	}


	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		VulkanDestory();
		VULKAN_CORE_WARN("[VULKAN] VertexBuffer Was Destoryed!");
	}

	void VulkanVertexBuffer::VulkanCreate()
	{
		MORP_PROFILE_FUNCTION();

		Ref<VulkanBuffer> StagingBuffer = VulkanBuffer::Create(VulkanBufferFlags::VulkanNone);
		VkBufferCreateInfo StagingCreateInfo = {};
		StagingCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		StagingCreateInfo.size = m_Size;
		StagingCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		StagingCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		StagingBuffer->Setup(StagingCreateInfo, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		StagingBuffer->SetData(m_Data, m_Size);
		
		m_Buffer = VulkanBuffer::Create(VulkanBufferFlags::VulkanDispatcherKeep);
		VkBufferCreateInfo BufferCreateInfo = {};
		BufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		BufferCreateInfo.size = m_Size;
		BufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		BufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		m_Buffer->Setup(BufferCreateInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		Vector<VkBufferCopy> CopyRegion = { VkBufferCopy{ 0, 0, m_Size } };
		Ref<VulkanCommand> SubmissionCommand = VulkanCommand::Create();
		SubmissionCommand->MakeCommand<CommandBeginBuffer>(VulkanCommandTypes::BeginBuffer);
		SubmissionCommand->MakeCommand<CommandCopyBufferToBuffer>(VulkanCommandTypes::CopyBufferToBuffer)->PopulateData(StagingBuffer, m_Buffer, CopyRegion);
		SubmissionCommand->MakeCommand<CommandEndBuffer>(VulkanCommandTypes::EndBuffer);

		Vector<Ref<VulkanResource>> Resources = { StagingBuffer, m_Buffer, SubmissionCommand, StagingBuffer };
		FindAvailableUnit()->MakeResources(Resources);
	}

	void VulkanVertexBuffer::VulkanDestory()
	{
	}

	Ref<VulkanVertexBuffer> VulkanVertexBuffer::Make(QuadVertex* _Data, const uint32& _Size)
	{
		return CreateRef<VulkanVertexBuffer>(_Data, _Size);
	}

}}