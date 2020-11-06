#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Renderer/RendererCore/RendererAPI.h"

#include "Platform/Vulkan/VulkanResources/VulkanCommand.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanRenderQueue.h"


namespace Morpheus {

	class VulkanRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void Flush() override;

		virtual void SetCompile(const bool& _Value) override;
		virtual void SetReady(const bool& _Value) override;

		virtual void SetClearColor(const Vector4& _ClearColor) override;

	private:
		void Resize();

	private:
		Ref<VulkanCommand> m_Command;
		Ref<VulkanRenderQueue> m_RenderQueue;
		Vector4 m_ClearColor = { 0.25f, 0.25f, 0.25f, 1.00f };
		bool m_ReadyRender = true;
	};


}