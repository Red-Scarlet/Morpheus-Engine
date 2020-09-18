#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Renderer/RendererAPI.h"

#include "Platform/Vulkan/VulkanCore/VulkanInstance.h"
#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"
#include "Platform/Vulkan/VulkanCore/VulkanPresentation.h"

#include "Morpheus/Renderer/RendererCore/Renderpass.h"
#include "Morpheus/Renderer/RendererCore/Pipeline.h"

#include "Platform/Vulkan/VulkanCommand/VulkanCommandSystem.h"
#include "Platform/Vulkan/VulkanCommand/VulkanSynchronization.h"

namespace Morpheus {

	class VulkanRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void Shutdown() override;

		virtual void Begin() override;
		virtual void End() override;
		virtual void Flush() override;
		virtual void Reset() override;

		virtual void SetViewport() override;
		virtual void SetClearColor(const Vector4& _Color) override;

		virtual void BindPipeline(const Ref<Pipeline>& _Pipeline) override;
		virtual void BeginRenderpass(const Ref<Renderpass>& _Renderpass) override;
		virtual void EndRenderpass(const Ref<Renderpass>& _Renderpass) override;

		virtual void DrawGeomerty(const Ref<VertexBuffer>& _VertexBuffer) override;

	private:
		struct {
			VulkanInstance* Instance;
			VulkanPresentation* Presentation;
			VulkanLogicalDevice* Device;
		} m_Vulkan;

		struct {
			VulkanCommandSystem* System;
			VulkanSynchronization* Synchronization;
		} m_Command;
		

	};

}