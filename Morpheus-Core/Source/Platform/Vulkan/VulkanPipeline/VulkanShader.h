#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"
#include "Platform/Vulkan/VulkanCache.h"

#include "Platform/Vulkan/VulkanCore/VulkanDevice.h"
#include "Platform/Vulkan/VulkanCore/VulkanSwapchain.h"
#include "Platform/Vulkan/VulkanResources/VulkanDescriptor.h"
#include "Platform/Vulkan/VulkanResources/VulkanUniformBuffer.h"
#include "Platform/Vulkan/VulkanResources/VulkanTextureBuffer.h"

#include "Morpheus/Renderer/RendererBindables/Shader.h"

namespace Morpheus {

	#define VULKAN_SHADER_CACHE_TYPE VulkanCacheType(6)
	class VulkanShader : public Shader
	{
	public:
		typedef VulkanCacheMember<VulkanShader> ShaderCache;

	public:		
		VulkanShader(const Ref<Renderpass>& _Renderpass, const ShaderAttributeLayout& _ShaderLayout, 
			const String& _VertexPath, const String& _FragmentPath, const Ref<ShaderCache>& _Cache);
		virtual ~VulkanShader();
		virtual const uint32& GetID() const override { return m_ID; }

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void SetInt(const String& _Name, const uint32& _Value, const Boolean& _Flags) override;
		virtual void SetFloat(const String& _Name, const float32& _Value, const Boolean& _Flags) override;
		virtual void SetFloat3(const String& _Name, const Vector3& _Value, const Boolean& _Flags) override;
		virtual void SetFloat4(const String& _Name, const Vector4& _Value, const Boolean& _Flags) override;
		virtual void SetMat4(const String& _Name, const Matrix4& _Matrix, const Boolean& _Flags) override;
		void VulkanCompile(const uint32& _Index);

	public:
		void UpdateTextureBuffer(const Ref<VulkanTextureBuffer>& _TextureBuffer, const uint32& _Index);
		void UpdateUniformBuffer(const Ref<VulkanUniformBuffer>& _UniformBuffer, const uint32& _Index);

		const VkPipeline& GetPipeline() { return m_Pipeline; };
		const VkPipelineLayout& GetPipelineLayout() { return m_PipelineLayout; };
		const VkDescriptorSet& GetDescriptorSet(const uint32& _Index);

	private:
		void VulkanCreate();
		void VulkanDestory();

		void PipelineCreate();
		void PipelineDestory();

	private:
		Ref<VulkanLogicalDevice> m_Device;
		Ref<Renderpass> m_Renderpass;
		Ref<ShaderCache> m_Cache;

		// Shader Module
		String m_VertexFile;
		String m_FragmentFile;
		VkShaderModule m_VertModule;
		VkShaderModule m_FragModule;

		// Descriptor Set
		Vector<Vector<VkWriteDescriptorSet>> m_DescriptorWrites;
		Ref<VulkanDescriptor> m_Descriptor;

		// Pipeline Split into Segments to combind into one shader call the rasteriatation shader
		VkPipeline m_Pipeline;
		VkPipelineCache m_PipelineCache;
		VkPipelineLayout m_PipelineLayout;
		Boolean m_PipelineCreated = false;

		// ExtraUtils
		ShaderAttributeLayout m_ShaderLayout;
		mutable UnorderedMap<String, AnyData> m_PushCache;
		uint32 m_ID;

	public:
		static Ref<VulkanShader> Make(const Ref<Renderpass>& _Renderpass, const ShaderAttributeLayout& _ShaderLayout,
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