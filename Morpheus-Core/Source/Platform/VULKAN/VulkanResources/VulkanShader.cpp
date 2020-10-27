#include "Morppch.h"
#include "VulkanShader.h"

#include <fstream>
#include "Platform/VULKAN/VulkanCore/VulkanResource.h"

namespace Morpheus {

	VulkanShader::VulkanShader(const String& _VertexPath, const String& _FragmentPath)
	{
		m_Device = VulkanResourceCache::GetInstance()->Get<VulkanDevice>(VulkanResourceType::Device);
		m_DescriptorPool = VulkanDescriptorPool::VulkanCreate(1);

		CreateShader(_VertexPath, _FragmentPath);
		MORP_CORE_WARN("[VULKAN] Shader Was Created!");
	}

	VulkanShader::~VulkanShader()
	{
		MORP_CORE_WARN("[VULKAN] Shader Was Destoryed!");
	}

	void VulkanShader::Destory()
	{
		vk::Device Device = m_Device->GetLogicalDevice();

		Device.destroyShaderModule(m_VertModule);
		Device.destroyShaderModule(m_FragModule);
	}

	void VulkanShader::Bind()
	{

		//uint32 BindedVAOS = VulkanResourceCache::GetInstance()->GetBindingCount(VulkanBindables::VertexArray);
		uint32 NumUniforms = VulkanResourceCache::GetInstance()->GetResourceCount(VulkanResourceType::UniformBuffer);
		for (uint32 i = 0; i < NumUniforms; i++)
			m_DescriptorPool->UpdateDescriptorSet(i);

		// Check so see how many VAO are currently Binded.
			// Bind this Shader.
			// Get all the VAO currently binded to this shader.
			// Update all the descriptorSets With Uniform IDs;

		//

		// Check How many Vertex Array Are currently Binded
		// IF 2

		// BIND this shader. and Increase the descriptor Set Count.

		//
		//
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

		m_VertModule = Device.createShaderModule(
			vk::ShaderModuleCreateInfo(
				vk::ShaderModuleCreateFlags(),
				vertShaderCode.size(),
				(uint32_t*)vertShaderCode.data()
			)
		);

		m_FragModule = Device.createShaderModule(
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
		VulkanResourceCache::GetInstance()->Submit<VulkanShader>(VulkanResourceType::Shader, s_VulkanShader, s_VulkanShader->GetID());
		return s_VulkanShader;
	}

}

