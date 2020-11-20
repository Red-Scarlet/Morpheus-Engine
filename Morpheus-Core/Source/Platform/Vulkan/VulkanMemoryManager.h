#pragma once
#include "Morpheus/Core/Common.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanInstance.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSurface.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSwapchain.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanCommandSystem.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanQueue.h"

#include "Platform/Vulkan/VulkanResources/VulkanVertexBuffer.h"
#include "Platform/Vulkan/VulkanResources/VulkanIndexBuffer.h"

#include "Platform/Vulkan/VulkanBindables/VulkanBindingChain.h"
#include "Platform/Vulkan/VulkanBindables/VulkanFramebuffer.h"
#include "Platform/Vulkan/VulkanBindables/VulkanShader.h"
#include "Platform/Vulkan/VulkanBindables/VulkanVertexArray.h"
#include "Platform/Vulkan/VulkanResources/VulkanTextureBuffer.h"

namespace Morpheus {

	class VulkanMemoryManager
	{
	protected:
		VulkanMemoryManager();
		~VulkanMemoryManager();

	public:
		static VulkanMemoryManager* GetInstance();

		void SetVulkanInstance(const Ref<VulkanInstance>& _Vulkan) { m_Vulkan = _Vulkan; }
		void SetSurface(const Ref<VulkanSurface>& _Vulkan) { m_Surface = _Vulkan; }
		void SetDevice(const Ref<VulkanDevice>& _Vulkan) { m_Device = _Vulkan; }
		void SetSwapchain(const Ref<VulkanSwapchain>& _Vulkan) { m_Swapchain = _Vulkan; }

		void SetCommandSystem(const Ref<VulkanCommandSystem>& _Vulkan) { m_CommandSystem = _Vulkan; }
		void SetVulkanQueue(const Ref<VulkanQueue>& _Vulkan) { m_Queue = _Vulkan; }

		const Ref<VulkanInstance>& GetVulkanInstance() { return m_Vulkan; }
		const Ref<VulkanSurface>& GetSurface() { return m_Surface; }
		const Ref<VulkanDevice>& GetDevice() { return m_Device; }
		const Ref<VulkanSwapchain>& GetSwapchain() { return m_Swapchain; }

		const Ref<VulkanCommandSystem>& GetCommandSystem() { return m_CommandSystem; }
		const Ref<VulkanQueue>& GetQueue() { return m_Queue; }

		VulkanVertexBufferCache& GetVertexBufferCache() { return m_VertexBufferCache; }
		VulkanIndexBufferCache& GetIndexBufferCache() { return m_IndexBufferCache; }
		VulkanTextureBufferCache& GetTextureBufferCache() { return m_TextureBufferCache; }

		VulkanFrameBufferCache& GetFrameBufferCache() { return m_FrameBufferCache; }
		VulkanShaderCache& GetShaderCache() { return m_ShaderCache; }
		VulkanVertexArrayCache& GetVertexArrayCache() { return m_VertexArrayCache; }

		VulkanBindingChain& GetBindingChain() { return m_VulkanBindingChain; }

	private:
		void Shutdown();
		void Init();

	private:
		static VulkanMemoryManager* s_Instance;

		Ref<VulkanInstance> m_Vulkan;
		Ref<VulkanSurface> m_Surface;
		Ref<VulkanDevice> m_Device;
		Ref<VulkanSwapchain> m_Swapchain;

		Ref<VulkanCommandSystem> m_CommandSystem;
		Ref<VulkanQueue> m_Queue;

		VulkanVertexBufferCache m_VertexBufferCache;
		VulkanIndexBufferCache m_IndexBufferCache;
		VulkanTextureBufferCache m_TextureBufferCache;

		VulkanFrameBufferCache m_FrameBufferCache;
		VulkanShaderCache m_ShaderCache;
		VulkanVertexArrayCache m_VertexArrayCache;

		VulkanBindingChain m_VulkanBindingChain;

	};

}