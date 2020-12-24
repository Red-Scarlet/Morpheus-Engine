#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCore/VulkanResource.h"

namespace Morpheus { namespace Vulkan {

    enum class VulkanImageTypes : uint8
    { VulkanNone = 0, VulkanTextureBuffer };

	class VulkanImage : public ParentRef<VulkanImage>, public VulkanResource
    {
    public:
        VulkanImage() = default;

        void Accept(Ref<IResourceDispatcher>& _Dispatcher) override 
        { _Dispatcher->Dispatch(shared_from_this()); }

    public:
        static Ref<VulkanImage> Create()
        { return CreateRef<VulkanImage>(); }
    };

}}