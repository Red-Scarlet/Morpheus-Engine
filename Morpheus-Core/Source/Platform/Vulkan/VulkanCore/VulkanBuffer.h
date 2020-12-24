#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanCore/VulkanResource.h"

namespace Morpheus { namespace Vulkan {

    enum class VulkanBufferTypes : uint8
    { VulkanNone = 0, VulkanVertexBuffer, VulkanIndexBuffer, VulkanUniformBuffer };

    enum VulkanBufferFlags : uint8
    { VulkanNone = 0x0, VulkanDispatcherKeep = 0x1 };

    class VulkanBuffer : public ParentRef<VulkanBuffer>, public VulkanResource
    {
    public:
        VulkanBuffer(const VulkanBufferFlags& _Flags)
            : m_Flags(_Flags)
        {
        }

        void Accept(Ref<IResourceDispatcher>& _Dispatcher) override
        { _Dispatcher->Dispatch(shared_from_this()); }

        void Setup(const VkBufferCreateInfo& _CreateInfo, const VkMemoryPropertyFlagBits& _MemoryProperties)
        {
            MORP_PROFILE_FUNCTION();

            m_CreateInfo = _CreateInfo;
            m_MemoryProperties = _MemoryProperties;
        }

        void SetData(void* _Data, const uint32& _Size)
        { m_Data = _Data; m_Size = _Size; }

        void SetBufferAndMemory(const VkBuffer _Buffer, const VkDeviceMemory& _Memory)
        {
            MORP_PROFILE_FUNCTION();

            m_Buffer = _Buffer;
            m_Memory = _Memory;
        }

    public:
        const bool& GetCompiled()
        { return m_IsCompiled; }

        void SetCompiled(const bool& _IsCompiled = true) 
        { m_IsCompiled = _IsCompiled; }

        VkBuffer& GetBuffer()
        { return m_Buffer; }

        VkDeviceMemory& GetMemory()
        { return m_Memory; }

        const uint32& GetSize()
        { return m_Size; }

        void* GetData()
        { return m_Data; }

        const VkBufferCreateInfo& GetCreateInfo()
        { return m_CreateInfo; }

        const VkMemoryPropertyFlagBits& GetMemoryProperties()
        { return m_MemoryProperties; }

        const VulkanBufferFlags& GetFlags() { return m_Flags; }

    private:
        uint32 m_Size = 0;
        void* m_Data = nullptr;

        VkBuffer m_Buffer = nullptr;
        VkDeviceMemory m_Memory = nullptr;
        VkBufferCreateInfo m_CreateInfo = {};
        VkMemoryPropertyFlagBits m_MemoryProperties = {};

        bool m_IsCompiled = false;
        VulkanBufferFlags m_Flags;

    public:
        static Ref<VulkanBuffer> Create(const VulkanBufferFlags& _Flags)
        { return CreateRef<VulkanBuffer>(_Flags); }
    };
}}