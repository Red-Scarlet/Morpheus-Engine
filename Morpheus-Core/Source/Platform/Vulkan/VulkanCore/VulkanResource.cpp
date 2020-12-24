#include "Morppch.h"
#include "VulkanResource.h"

#include "Platform/Vulkan/VulkanCore/VulkanBuffer.h"
#include "Platform/Vulkan/VulkanCore/VulkanImage.h"
#include "Platform/Vulkan/VulkanCore/VulkanCommand.h"

namespace Morpheus { namespace Vulkan {

    ResourceDispatcher::ResourceDispatcher(const Ref<VulkanDevice>& _Device, const Ref<VulkanCommandPool>& _Pool)
        : m_Device(_Device), m_CommandPool(_Pool)
    {
        MORP_PROFILE_FUNCTION();
    }

    ResourceDispatcher::~ResourceDispatcher()
    {
        for (Ref<VulkanBuffer>& buffer : m_Buffers) {
            vkDestroyBuffer(m_Device->GetLogical(), buffer->GetBuffer(), nullptr);
            vkFreeMemory(m_Device->GetLogical(), buffer->GetMemory(), nullptr);
        }
    }

    void ResourceDispatcher::Dispatch(Ref<VulkanBuffer>& _Buffer)
    {
        MORP_PROFILE_FUNCTION();

        MORP_CORE_WARN("[RESOURCE] Dispatching VulkanBuffer!");

        if (_Buffer->GetCompiled()) {
           MORP_CORE_WARN("[RESOURCE] Destroyed VulkanBuffer!");
           vkDestroyBuffer(m_Device->GetLogical(), _Buffer->GetBuffer(), nullptr);
           vkFreeMemory(m_Device->GetLogical(), _Buffer->GetMemory(), nullptr);
           return;
        }

        uint32 Size = _Buffer->GetSize();
        VkBufferCreateInfo CreateInfo = _Buffer->GetCreateInfo();
        VkMemoryPropertyFlagBits MemoryProperties = _Buffer->GetMemoryProperties();

        void* Data = _Buffer->GetData();

        VkResult result = vkCreateBuffer(m_Device->GetLogical(), &CreateInfo, nullptr, &_Buffer->GetBuffer());
        MORP_CORE_ASSERT(result, "[VULKAN] CreateBuffer Failure!");

        VkMemoryRequirements MemoryRequirements = {};
        vkGetBufferMemoryRequirements(m_Device->GetLogical(), _Buffer->GetBuffer(), &MemoryRequirements);
        uint32 MemoryIndex = m_Device->FindMemoryType(MemoryRequirements.memoryTypeBits, MemoryProperties);
        // TODO: Create Additional Information Inside the VulkanBuffer Header!
        VkMemoryAllocateInfo AllocInfo = {};
        AllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        AllocInfo.allocationSize = MemoryRequirements.size;
        AllocInfo.memoryTypeIndex = MemoryIndex;
        result = vkAllocateMemory(m_Device->GetLogical(), &AllocInfo, nullptr, &_Buffer->GetMemory());
        MORP_CORE_ASSERT(result, "[VULKAN] AllocateMemory Failure!");

        result = vkBindBufferMemory(m_Device->GetLogical(), _Buffer->GetBuffer(), _Buffer->GetMemory(), 0);
        MORP_CORE_ASSERT(result, "[VULKAN] BindBufferMemory Failure!");

        if (Data != nullptr) {
            void* pData = nullptr;
            vkMapMemory(m_Device->GetLogical(), _Buffer->GetMemory(), 0, Size, 0, &pData);
            memcpy(pData, Data, Size);
            vkUnmapMemory(m_Device->GetLogical(), _Buffer->GetMemory());
        }

        _Buffer->SetCompiled(true);

        if(_Buffer->GetFlags() & VulkanBufferFlags::VulkanDispatcherKeep)
            m_Buffers.push_back(_Buffer);
    }

    void ResourceDispatcher::Dispatch(Ref<VulkanImage>& _Image)
    {
        MORP_PROFILE_FUNCTION();

        MORP_CORE_WARN("[RESOURCE] Dispatching VulkanImage!");
        // Make Function on VulkanCall
    }

    void ResourceDispatcher::Dispatch(Ref<VulkanCommand>& _Command)
    {
        MORP_PROFILE_FUNCTION();

        MORP_CORE_WARN("[RESOURCE] Dispatching VulkanCommand!");
        // Make Function on VulkanCall

        VkCommandBuffer Buffer = m_CommandPool->GetCommandBuffer();
        Ref<VulkanCommandBuffer> CommandBuffer = CreateRef<VulkanCommandBuffer>(Buffer);

        Vector<Ref<VulkanInstruction>> Instructions = _Command->GetCommands();
        for (Ref<VulkanInstruction>& Instr : Instructions)
            Instr->Execute(CommandBuffer);
        _Command->Clear();


        VkSubmitInfo SubmitInfo = {};
        SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        SubmitInfo.commandBufferCount = 1;
        SubmitInfo.pCommandBuffers = &Buffer;

        VkFenceCreateInfo CreateInfo = {};
        CreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
       
        VkFence Fence = {};
        vkCreateFence(m_Device->GetLogical(), &CreateInfo, nullptr, &Fence);
        Ref<VulkanQueue> Queue = m_Device->GetQueue(QueueType::VULKAN_QUEUE_GRAPHICS);
        Queue->Submit(&SubmitInfo, Fence);
        Queue->Wait();

        vkWaitForFences(m_Device->GetLogical(), 1, &Fence, VK_TRUE, UINT_MAX);
        vkDestroyFence(m_Device->GetLogical(), Fence, nullptr);
    }

    Ref<ResourceDispatcher> ResourceDispatcher::Create(const Ref<VulkanDevice>& _Device, const Ref<VulkanCommandPool>& _Pool)
    {
        return CreateRef<ResourceDispatcher>(_Device, _Pool);
    }

}}