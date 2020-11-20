#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/Vulkan/VulkanCommon.h"

#include "Morpheus/Renderer/RendererBindables/Shader.h"
#include "Platform/Vulkan/VulkanBindables/VulkanFramebuffer.h"
#include "Platform/Vulkan/VulkanGlobals/VulkanDevice.h"

#include <unordered_map>

#define SHADER_MAX_DESCRIPTOR_POOLSIZE 16

namespace Morpheus {

	struct VulkanUniformBuffer
	{
	public:
		VulkanBuffer Buffer;	// TODO BUILD THE SIZE INTO VULKANBUFFER
		uint32 Size = 0;
		uint32 DescriptorIndex = 0;
	};

	class VulkanUniformMessage
	{
	public:
		Vector<uint8> Data;
		
	public:
		template<typename DataType>
		friend VulkanUniformMessage& operator<< (VulkanUniformMessage& _Message, const DataType& _Data)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");
			
			uint32 i = _Message.Data.size();
			_Message.Data.resize(_Message.Data.size() + sizeof(DataType));
			std::memcpy(_Message.Data.data() + i, &_Data, sizeof(DataType));
			
			return _Message;
		}

		template<typename DataType>
		friend VulkanUniformMessage& operator>> (VulkanUniformMessage& _Message, DataType& _Data)
		{
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");
			
			uint32 i = _Message.Data.size() - sizeof(DataType);
			std::memcpy(&_Data, _Message.Data.data() + i, sizeof(DataType));
			_Message.Data.resize(i);

			return _Message;
		}
	};

	class VulkanShader : public Shader
	{
	public:
		VulkanShader(const ShaderAttributeLayout& _Layout, const String& _VertexPath, const String& _FragmentPath);
		virtual ~VulkanShader();
		
		virtual void Bind() override;
		virtual void Unbind() override;

		virtual const uint32& GetID() const override { return m_ID; }

		virtual void SetUniformDescription(const ShaderAttributeLayout& _Layout) override { m_UniformLayout = _Layout; }
		virtual void SetLayout(const ShaderAttributeLayout& _Layout) override { m_Layout = _Layout; }
		virtual const ShaderAttributeLayout& GetLayout() override { return m_Layout; };
		
		virtual void SetInt(const String& _Name, const uint32& _Value) override;
		virtual void SetFloat(const String& _Name, const float32& _Value) override;
		virtual void SetFloat3(const String& _Name, const Vector3& _Value) override;
		virtual void SetFloat4(const String& _Name, const Vector4& _Value) override;
		virtual void SetMat4(const String& _Name, const Matrix4& _Matrix) override;

		const vk::Pipeline& GetPipeline() { return m_Pipeline; };
		const vk::PipelineLayout& GetPipelineLayout() { return m_PipelineLayout; };
		const vk::DescriptorSet& GetDescriptorSet(const uint32& _Index) { return m_DescriptorSets[_Index]; }
		const uint32& GetDescriptorCount() 
		{ 
			return m_DescriptorSets.size(); 
		}

	public:
		void SetupPipeline(const uint32& _FrameBufferID);
		uint32 SetupUniform();

		void CompileUniform(const uint32& _Index);

	private:
		void UploadUniformInt(const String& _Name, const int32& _Int);
		void UploadUniformFloat(const String& _Name, const float32& _Float);
		void UploadUniformFloat2(const String& _Name, const Vector2& _Float2);
		void UploadUniformFloat3(const String& _Name, const Vector3& _Float3);
		void UploadUniformFloat4(const String& _Name, const Vector4& _Float4);
		void UploadUniformMat4(const String& _Name, const Matrix4& _Matrix);

		void CreateShader();
		void CreateDescriptorLayout();
		void CreatePipeline(const uint32& _FrameBufferID);
		void DestoryDescriptorLayout();
		void DestoryPipeline();

		void UpdateDescriptorSet(const VulkanUniformBuffer& _Buffer);

		void CreateUniformBuffer();
		void UpdateUniformBuffer(const uint32& _Index);
		void DestoryUniformBuffer();

		void ExpandDescriptorArray();
		void AllocateDescriptorSet();

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanSwapchain> m_Swapchain;

		// Shader
		vk::ShaderModule m_VertModule;
		vk::ShaderModule m_FragModule;
		String m_VertexFile;
		String m_FragmentFile;

		// NewDescriptorSet Arch
		vk::DescriptorSetLayout m_DescriptorLayout;

		Vector<vk::DescriptorPool> m_DescriptorArray;
		Vector<vk::DescriptorSet> m_DescriptorSets;
		uint32 m_DescriptorCount = 0;
		uint32 m_DescriptorSize = 0;

		// Pipeline
		vk::Pipeline m_Pipeline;
		vk::PipelineCache m_PipelineCache;
		vk::PipelineLayout m_PipelineLayout;
		vk::PipelineVertexInputStateCreateInfo m_InputState;
		Boolean m_PipelineCreated = false;
		
		// Uniform
		Vector<VulkanUniformBuffer> m_UniformBuffers;
		uint32 m_UniformCount = 0;

		// ExtraUtils
		ShaderAttributeLayout m_Layout;
		ShaderAttributeLayout m_UniformLayout;

		uint32 m_MessageStack = 0;
		mutable UnorderedMap<String, AnyData> m_UniformCache;

		
		uint32 m_ID;

	public:
		static Ref<VulkanShader> Make(const ShaderAttributeLayout& _Layout, const String& _VertexPath, const String& _FragmentPath);
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
