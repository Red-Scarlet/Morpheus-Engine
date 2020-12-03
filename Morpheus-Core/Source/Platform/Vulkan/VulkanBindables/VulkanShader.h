#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanSwapchain.h"
#include "Platform/Vulkan/VulkanResources/VulkanDescriptor.h"
#include "Platform/Vulkan/VulkanResources/VulkanUniformBuffer.h"
#include "Platform/Vulkan/VulkanResources/VulkanTextureBuffer.h"

#include "Morpheus/Renderer/RendererBindables/Shader.h"

namespace Morpheus {

	class VulkanShader : public Shader
	{
	public:		
		VulkanShader(const ShaderAttributeLayout& _ShaderLayout, const ShaderAttributeLayout& _UniformLayout,
			const String& _VertexPath, const String& _FragmentPath);
		virtual ~VulkanShader();
		virtual const uint32& GetID() const override { return m_ID; }

		virtual void Bind(const uint32& _Slot) override;
		virtual void Unbind() override;

		virtual void SetInt(const String& _Name, const uint32& _Value, const Boolean& _Flags) override;
		virtual void SetFloat(const String& _Name, const float32& _Value, const Boolean& _Flags) override;
		virtual void SetFloat3(const String& _Name, const Vector3& _Value, const Boolean& _Flags) override;
		virtual void SetFloat4(const String& _Name, const Vector4& _Value, const Boolean& _Flags) override;
		virtual void SetMat4(const String& _Name, const Matrix4& _Matrix, const Boolean& _Flags) override;
		virtual void SetSampler(const String& _Name, const uint32& _Value, const Boolean& _Flags) override;

		void VulkanCompile(const uint32& _Index);

	public:
		void UpdateTextureBuffer(const Ref<VulkanTextureBuffer>& _TextureBuffer, const uint32& _Index);
		void UpdateUniformBuffer(const Ref<VulkanUniformBuffer>& _UniformBuffer);
		void DeallocateUniformBuffer(const Ref<VulkanUniformBuffer>& _UniformBuffer);
		Ref<VulkanUniformBuffer> AllocateUniformBuffer(const uint32& _Index);

		const vk::Pipeline& GetPipeline() { return m_Pipeline; };
		const vk::PipelineLayout& GetPipelineLayout() { return m_PipelineLayout; };
		
	private:
		void VulkanCreate();
		void VulkanDestory();

		void PipelineCreate(const uint32& _FrameBufferID);
		void PipelineDestory();

	private:
		Ref<VulkanDevice> m_Device;

		// Shader Module
		String m_VertexFile;
		String m_FragmentFile;
		vk::ShaderModule m_VertModule;
		vk::ShaderModule m_FragModule;

		// Descriptor Set
		Vector<Vector<vk::WriteDescriptorSet>> m_DescriptorWrites;
		Ref<VulkanDescriptor> m_Descriptor;
		uint32 m_UniformBufferCount = 0;

		// Pipeline Split into Segments to combind into one shader call the rasteriatation shader
		vk::Pipeline m_Pipeline;
		vk::PipelineCache m_PipelineCache;
		vk::PipelineLayout m_PipelineLayout;
		vk::PipelineVertexInputStateCreateInfo m_InputState;
		Boolean m_PipelineCreated = false;

		// ExtraUtils
		ShaderAttributeLayout m_ShaderLayout;
		ShaderAttributeLayout m_UniformLayout;

		mutable UnorderedMap<String, AnyData> m_UniformCache;

		uint32 m_ID;

	public:
		static Ref<VulkanShader> Make(const ShaderAttributeLayout& _ShaderLayout, const ShaderAttributeLayout& _UniformLayout,
			const String& _VertexPath, const String& _FragmentPath);
	};

	class VulkanShaderCache
	{
	public:
		void Add(const uint32& _ID, const Ref<VulkanShader>& _Shader)
		{ m_Cache[_ID] = _Shader; m_Count++; }

		const Ref<VulkanShader>& Get(const uint32& _ID)
		{
			auto it = m_Cache.find(_ID);
			if (it != m_Cache.end())
				return it->second;
			MORP_CORE_ASSERT(MORP_ERROR, "[VULKAN] Could not find Shader #" + std::to_string(_ID) + " in Cache!");
			return nullptr;
		}

		bool Exists(const uint32& _ID) const
		{ return m_Cache.find(_ID) != m_Cache.end(); }

		const uint32& Count() const { return m_Count; }

	public:
		UnorderedMap<uint32, Ref<VulkanShader>> m_Cache;
		uint32 m_Count = 0;
	};

}