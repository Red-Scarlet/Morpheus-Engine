#pragma once

#include "Morpheus/Core/Common.h"

#include "Platform/Vulkan/VulkanCore/VulkanInstance.h"
#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"
#include "Platform/Vulkan/VulkanCore/VulkanUnit.h"

#include "Morpheus/Renderer/RendererCore/GraphicsContext.h"

namespace Morpheus { namespace Vulkan {

	class VulkanContext : public GraphicsContext
	{
	public:
		VulkanContext() = default;
		~VulkanContext() = default;

		virtual void Init() override;
		virtual void InitUnit() override;
		virtual void Shutdown() override;

		virtual void SetUnit(const uint32& _ID, const ThreadID& _TID) override;

		virtual void Flush() override; 
		virtual void FlushUnit() override;

		virtual void TestSomething() override;

	private:
		Ref<VulkanInstance> m_Instance;
		Ref<VulkanDevice> m_Device;
		Ref<VulkanUnit::UnitCache> m_UnitCache;
		ThreadID m_MainThreadID;

		static bool s_Running;
	};

}}