#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Morpheus/ResourceManager/ResourceSystem.h"
#include "Morpheus/ResourceManager/ResourceCommand.h"

namespace Morpheus { namespace Vulkan {

    // Used to Update/Change Resources
    class VulkanRenderer : public ResourceSystem
    {
    public:
        VulkanRenderer();
        ~VulkanRenderer() {}

        virtual void Init();
        virtual void Update() override;
        virtual void Stop();

    private:
        bool Process(const Resource& _Resource);

    public:
        static Ref<VulkanRenderer> Create();
    };

}}