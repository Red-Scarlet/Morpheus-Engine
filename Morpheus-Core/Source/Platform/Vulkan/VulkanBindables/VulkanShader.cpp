#include "Morppch.h"
#include "VulkanShader.h"

#include "Platform/Vulkan/VulkanMemoryManager.h"
#include <fstream>

#include "VulkanVertexArray.h"

namespace Morpheus {

	VulkanShader::VulkanShader(const String& _VertexPath, const String& _FragmentPath)
		: m_DescriptorCount(1)
	{
		m_Device = VulkanMemoryManager::GetInstance()->GetGlobalCache()->Get<VulkanDevice>(VulkanGlobalTypes::VulkanDevice);
		m_DescriptorPool = VulkanDescriptorPool::VulkanCreate(m_DescriptorCount);
		m_Identifier.Type = VulkanBindableTypes::VulkanShader;

		CreateShader(_VertexPath, _FragmentPath);
		SetID(VulkanMemoryManager::GetInstance()->GetBindableCache()->GetNextBindableID(VulkanBindableTypes::VulkanShader));
		MORP_CORE_WARN("[VULKAN] Shader Was Created!");
	}

	VulkanShader::~VulkanShader()
	{
		MORP_CORE_WARN("[VULKAN] Shader Was Destoryed!");
	}

	void VulkanShader::Destory()
	{
		vk::Device Device = m_Device->GetLogicalDevice();

		Device.destroyShaderModule(m_ShaderModules.VertModule);
		Device.destroyShaderModule(m_ShaderModules.FragModule);
	}

	void VulkanShader::Bind()
	{
		String str = "[VULKAN] Shader #" + std::to_string(m_Identifier.ID) + " was Binded!";
		MORP_CORE_TRACE(str);
		// Maybe added Message
		Ref<VulkanBindableCache> BindableCache = VulkanMemoryManager::GetInstance()->GetBindableCache();
		if (BindableCache->CheckBinding(m_Identifier.Type, m_Identifier.ID)) {
			// Already Bound!
		} else {
			BindableCache->SetPresent(m_Identifier);
			for (uint32 i = 0; i < BindableCache->AppendingCount(); i++) {
				VulkanBindableIdentifier Identifier = BindableCache->GetAppending(i);
				if (Identifier.Bounded) {
					AddToBindables(Identifier);
					Ref<VulkanVertexArray> VAO = BindableCache->Get<VulkanVertexArray>(VulkanBindableTypes::VulkanVertexArray, Identifier.ID);
					CompileUniform(VAO->GetUniformBuffer()->GetID());
				}
			}
			BindableCache->ClearAppending();
		}	
	}

	void VulkanShader::AddToBindables(const VulkanBindableIdentifier& _Identifer)
	{
		m_Bindables.push_back(_Identifer);
	}

	void VulkanShader::CompileUniform(const uint32& _ID)
	{
		if (m_DescriptorCount > m_DescriptorPool->GetCount()) {
			{
				m_DescriptorPool->Destory();
				m_DescriptorPool->SetDesciptorCount(m_DescriptorCount);
				m_DescriptorPool->CreateDescriptorPool();
				m_DescriptorPool->CreateDescriptorSet();
			}

			for (uint32 i = 0; i < m_Bindables.size(); i++) {
				Ref<VulkanBindableCache> BindableCache = VulkanMemoryManager::GetInstance()->GetBindableCache();
				Ref<VulkanVertexArray> VAO = BindableCache->Get<VulkanVertexArray>(VulkanBindableTypes::VulkanVertexArray, m_Bindables[i].ID);
				if(VAO->GetUniformBuffer()->GetID() != _ID) {
					Ref<VulkanUniformBuffer> UBO = VulkanMemoryManager::GetInstance()->GetResourceCache()->Get<VulkanUniformBuffer>(VulkanResourceTypes::VulkanUniformBuffer, VAO->GetUniformBuffer()->GetID());
					UBO->SetCompiled(false);
					m_DescriptorPool->UpdateDescriptorSet(UBO->GetID());
				}
			}

		}
	
		m_DescriptorPool->UpdateDescriptorSet(_ID); 
		m_DescriptorCount++;

		CheckVertexArray();
	}

	void VulkanShader::CheckVertexArray()
	{
		if (!m_Checked) {	
			Ref<VulkanBindableCache> BindableCache = VulkanMemoryManager::GetInstance()->GetBindableCache();
			Ref<VulkanVertexArray> VAO = BindableCache->Get<VulkanVertexArray>(VulkanBindableTypes::VulkanVertexArray, m_Bindables[0].ID);
			m_Pipeline = VulkanPipeline::VulkanCreate(m_ShaderModules, VAO->GetInputState());
			m_Checked = true;
		}
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

	void VulkanShader::CreateShader(const String& _VertexPath, const String& _FragmentPath)
	{	
		vk::Device Device = m_Device->GetLogicalDevice();

		Vector<float8> vertShaderCode = ReadFile(_VertexPath);
		Vector<float8> fragShaderCode = ReadFile(_FragmentPath);

		m_ShaderModules.VertModule = Device.createShaderModule(
			vk::ShaderModuleCreateInfo(
				vk::ShaderModuleCreateFlags(),
				vertShaderCode.size(),
				(uint32_t*)vertShaderCode.data()
			)
		);

		m_ShaderModules.FragModule = Device.createShaderModule(
			vk::ShaderModuleCreateInfo(
				vk::ShaderModuleCreateFlags(),
				fragShaderCode.size(),
				(uint32_t*)fragShaderCode.data()
			)
		);
	}

	Ref<VulkanShader> VulkanShader::VulkanCreate(const String& _VertexPath, const String& _FragmentPath)
	{
		Ref<VulkanShader> s_VulkanShader = CreateRef<VulkanShader>(_VertexPath, _FragmentPath);
		VulkanMemoryManager::GetInstance()->GetBindableCache()->Submit<VulkanShader>(VulkanBindableTypes::VulkanShader, s_VulkanShader);
		return s_VulkanShader;
	}

}

