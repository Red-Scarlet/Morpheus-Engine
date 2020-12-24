#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"
#include "Platform/Vulkan/VulkanCommand/VulkanCommandPool.h"

namespace Morpheus { namespace Vulkan {

    class IResourceDispatcher;
    class VulkanBuffer;
    class VulkanImage;
    class VulkanCommand;

    class VulkanResource
    { 
    public:
        virtual void Accept(Ref<IResourceDispatcher>& _Dispatcher) = 0;

    };

    class IResourceDispatcher
    { 
    public:
        virtual void Dispatch(Ref<VulkanBuffer>& _Resource) = 0;
        virtual void Dispatch(Ref<VulkanImage>& _Resource) = 0;
        virtual void Dispatch(Ref<VulkanCommand>& _Resource) = 0;

    };

    class ResourceDispatcher : public IResourceDispatcher
    {
    public:
        ResourceDispatcher(const Ref<VulkanDevice>& _Device, const Ref<VulkanCommandPool>& _Pool);
        ~ResourceDispatcher();
        
        void Dispatch(Ref<VulkanBuffer>& _Buffer) override;
        void Dispatch(Ref<VulkanImage>& _Image) override;
        void Dispatch(Ref<VulkanCommand>& _Command) override;

    private:
        Ref<VulkanDevice> m_Device;
        Ref<VulkanCommandPool> m_CommandPool;
        Vector<Ref<VulkanBuffer>> m_Buffers;

    public:
        static Ref<ResourceDispatcher> Create(const Ref<VulkanDevice>& _Device, const Ref<VulkanCommandPool>& _Pool);
    };
}}