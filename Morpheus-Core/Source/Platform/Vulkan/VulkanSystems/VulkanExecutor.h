#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Morpheus/ResourceManager/ResourceSystem.h"
#include "Morpheus/ResourceManager/ResourceCommand.h"

namespace Morpheus { namespace Vulkan {

    // Used to Execute/Build Commands for Resources
    class VulkanExecutor : public ResourceSystem
    {
    public:
        VulkanExecutor();
        ~VulkanExecutor() {}

        virtual void Init();
        virtual void Update() override;
        virtual void Stop();

    private:
        bool Process(const Resource& _Resource);

    public:
        static Ref<VulkanExecutor> Create();
    };

}}