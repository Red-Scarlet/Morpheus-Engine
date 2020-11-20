#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Renderer/RendererCore/RendererAPI.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanQueue.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanCommandSystem.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanCommand/VulkanCommandBuffer.h"


namespace Morpheus {

	class VulkanRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void Shutdown() override;
		virtual void SetClearColor(const Vector4& _ClearColor) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& _VertexArray) override;
		
		virtual void Flush() override;

	private:
		void SetupCommands();

	private:
		Ref<VulkanQueue> m_Queue;
		Ref<VulkanCommandSystem> m_CommandSystem;
		//UnorderedMap<uint32, VulkanCommands> m_Commands;


		Vector<uint32> m_VertexArrays;
		VulkanCommands m_Commands;
		Boolean m_Compilation = false;

		Vector4 m_ClearColor = { 0.25f, 0.25f, 0.25f, 1.00f };
	};


}