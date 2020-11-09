#include "Morppch.h"
#include "VulkanShader.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"
#include <fstream>

#include "VulkanVertexArray.h"
#include "VulkanFramebuffer.h"

namespace Morpheus {

	VulkanShader::VulkanShader(const VertexAttributeLayout& _Layout, const ShaderInfo& _ShaderPath)
		: VulkanBindable(VulkanBindableTypes::VulkanShader), m_Layout(_Layout), m_ShaderPath(_ShaderPath), m_DescriptorCount(1)
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanDevice>(VulkanGlobalTypes::VulkanDevice);
		SetID(VulkanMemoryManager::GetInstance()->GetBindableCache()->GetNextBindableID(VulkanBindableTypes::VulkanShader));

		VulkanCreate();
		MORP_CORE_WARN("[VULKAN] Shader #" + std::to_string(GetID()) + " Was Created!");
	}

	VulkanShader::~VulkanShader()
	{
		MORP_CORE_WARN("[VULKAN] Shader Was Destoryed!");
	}

	void VulkanShader::VulkanCreate()
	{
		vk::Device Device = m_Device->GetLogicalDevice();

		Vector<float8> vertShaderCode = ReadFile(m_ShaderPath.VertexPath);
		Vector<float8> fragShaderCode = ReadFile(m_ShaderPath.FragmentPath);

		m_ShaderModules.VertModule = Device.createShaderModule(
			vk::ShaderModuleCreateInfo(
				vk::ShaderModuleCreateFlags(),
				vertShaderCode.size(),
				(uint32*)vertShaderCode.data()
			)
		);

		m_ShaderModules.FragModule = Device.createShaderModule(
			vk::ShaderModuleCreateInfo(
				vk::ShaderModuleCreateFlags(),
				fragShaderCode.size(),
				(uint32*)fragShaderCode.data()
			)
		);

		m_DescriptorPool = VulkanDescriptorPool::Make(m_DescriptorCount);

		{
			m_Attributes.InputBinding.binding = 0;
			m_Attributes.InputBinding.stride = m_Layout.GetStride();
			m_Attributes.InputBinding.inputRate = vk::VertexInputRate::eVertex;
		}

		uint32 Index = 0;
		for (const VertexAttributeElement& Element : m_Layout) {
			vk::VertexInputAttributeDescription Attribute = {};
			Attribute.binding = 0;
			Attribute.location = Index;
			Attribute.format = vk::Format::eR32G32B32Sfloat;
			Attribute.offset = Element.Offset;
			m_Attributes.InputAttributes.push_back(Attribute);
			Index++;
		}

		{
			m_Attributes.InputState.flags = vk::PipelineVertexInputStateCreateFlags();
			m_Attributes.InputState.vertexBindingDescriptionCount = 1;
			m_Attributes.InputState.pVertexBindingDescriptions = &m_Attributes.InputBinding;
			m_Attributes.InputState.vertexAttributeDescriptionCount = m_Attributes.InputAttributes.size();
			m_Attributes.InputState.pVertexAttributeDescriptions = m_Attributes.InputAttributes.data();
		}

		m_Pipeline = VulkanPipeline::Make(m_ShaderModules, m_Attributes.InputState);
	}

	void VulkanShader::VulkanDestory()
	{
		vk::Device Device = m_Device->GetLogicalDevice();

		Device.destroyShaderModule(m_ShaderModules.VertModule);
		Device.destroyShaderModule(m_ShaderModules.FragModule);
	}

	void VulkanShader::Bind()
	{
		MORP_CORE_TRACE("[VULKAN] Shader #" + std::to_string(GetID()) + " was Binded!");
		Ref<VulkanBindableCache> BindableCache = VulkanMemoryManager::GetInstance()->GetBindableCache();
		BindableCache->DisableBindings(VulkanBindableTypes::VulkanShader);
		SetBound(true);

		for (uint32 i = 0; i < BindableCache->GetAppendingCount(VulkanBindableTypes::VulkanFramebuffer); i++)
		{
			auto Framebuffer = BindableCache->Get<VulkanFramebuffer>(VulkanBindableTypes::VulkanFramebuffer, i);
			if (Framebuffer->GetBound() && Framebuffer->CheckAppening()) {
				Framebuffer->DisableAppening();
				AppendBinding(Framebuffer->GetIdentifier());
			}
		}

		for (uint32 i = 0; i < BindableCache->GetAppendingCount(VulkanBindableTypes::VulkanVertexArray); i++) {
			auto VertexArray = BindableCache->Get<VulkanVertexArray>(VulkanBindableTypes::VulkanVertexArray, i);
			if (VertexArray->GetBound() && VertexArray->CheckAppening()) {
				VertexArray->DisableAppening();
				AppendBinding(VertexArray->GetIdentifier());
				CompileUniform(VertexArray->GetUniformBufferID());
			}
		}
	}

	void VulkanShader::CompileUniform(const uint32& _ID)
	{
		if (m_DescriptorCount > m_DescriptorPool->GetCount()) {
			{
				m_DescriptorPool->FreeDescriptor();
				m_DescriptorPool->SetDesciptorCount(m_DescriptorCount);
				m_DescriptorPool->CreateDescriptorPool();
				m_DescriptorPool->CreateDescriptorSet();
			}
		
			for (uint32 i = 0; i < GetBindingsCount(VulkanBindableTypes::VulkanVertexArray); i++) {
				Ref<VulkanBindableCache> BindableCache = VulkanMemoryManager::GetInstance()->GetBindableCache();
				auto VertexArray = BindableCache->Get<VulkanVertexArray>(VulkanBindableTypes::VulkanVertexArray, GetBinding(VulkanBindableTypes::VulkanVertexArray, i).ID);
				if (VertexArray->GetUniformBufferID() != _ID) {
					Ref<VulkanUniformBuffer> UBO = VulkanMemoryManager::GetInstance()->GetResourceCache()->Get<VulkanUniformBuffer>(VulkanResourceTypes::VulkanUniformBuffer, VertexArray->GetUniformBufferID());
					UBO->SetCompiled(false);
					m_DescriptorPool->UpdateDescriptorSet(UBO->GetID());
				}
			}
		
		}
		
		m_DescriptorPool->UpdateDescriptorSet(_ID); 
		m_DescriptorCount++;
	}

	Vector<float8> VulkanShader::ReadFile(const String& _Filepath)
	{
		std::ifstream file(_Filepath, std::ios::ate | std::ios::binary);

		if (!file.is_open())
			MORP_CORE_ASSERT(true, "Failed to open file!");

		uint32 fileSize = (uint32)file.tellg();
		Vector<char> buffer(fileSize);
		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();

		return buffer;
	}


	Ref<VulkanShader> VulkanShader::Make(const VertexAttributeLayout& _Layout, const ShaderInfo& _ShaderPath)
	{
		Ref<VulkanShader> s_VulkanShader = CreateRef<VulkanShader>(_Layout, _ShaderPath);
		VulkanMemoryManager::GetInstance()->GetBindableCache()->Submit(s_VulkanShader);
		return s_VulkanShader;
	}

}

