#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Morpheus/ResourceManager/ResourceSystem.h"
#include "Morpheus/ResourceManager/ResourceCommand.h"

namespace Morpheus { namespace Vulkan {

    // Used to Update/Change Resources
    class VulkanUpdator : public ResourceSystem
    {
    public:
        VulkanUpdator();
        ~VulkanUpdator() {}

        virtual void Init();
        virtual void Update() override;
        virtual void Stop();

    private:
        void Process(const Resource& _Resource);

    public:
        static Ref<VulkanUpdator> Create();
    };

}}