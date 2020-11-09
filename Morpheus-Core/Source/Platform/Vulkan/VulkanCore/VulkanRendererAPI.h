#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Renderer/RendererCore/RendererAPI.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanQueue.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanCommand/VulkanCommandSystem.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanCommand/VulkanCommandBuffer.h"


namespace Morpheus {

	class VulkanRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void Flush() override;

		virtual void SetClearColor(const Vector4& _ClearColor) override;
		virtual void SetCompile() override;

	private:
		void SetupCommands();

	private:
		Ref<VulkanQueue> m_Queue;
		Ref<VulkanCommandSystem> m_CommandSystem;
		VulkanCommands m_CommandBuffers;

		Vector4 m_ClearColor = { 0.25f, 0.25f, 0.25f, 1.00f };
		bool m_Compiled = false;
	};


}