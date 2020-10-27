#pragma once

#include "Morpheus/Core/Common.h"
#include "Platform/VULKAN/VulkanCommon.h"
#include "Morpheus/Renderer/RendererResources/Shader.h"

#include "Platform/VULKAN/VulkanDevice.h"
#include "Platform/VULKAN/VulkanResources/VulkanDescriptorPool.h"
#include "Platform/VULKAN/VulkanResources/VulkanUniformBuffer.h"

namespace Morpheus {

	// HARD CODED DIRECTORIES

	class VulkanShader : public Shader
	{
	public:
		VulkanShader(const String& _VertexPath, const String& _FragmentPath);
		virtual ~VulkanShader();
		void Destory();

		virtual void Bind() override;

		const uint32& GetID() { return m_ID; }
		void SetID(const uint32& _ID) { m_ID = _ID; }

		const vk::ShaderModule& GetVertexShader() { return m_VertModule; }
		const vk::ShaderModule& GetPixelShader() { return m_FragModule; }

	private:
		Vector<float8> ReadFile(const String& _Filepath);
		void CreateShader(const String& _VertexPath, const String& _FragmentPath);

	private:
		Ref<VulkanDevice> m_Device;
		Ref<VulkanDescriptorPool> m_DescriptorPool;

		vk::ShaderModule m_VertModule;
		vk::ShaderModule m_FragModule;
		uint32 m_ID = 0;

	public:
		static Ref<VulkanShader> VulkanCreate(const String& _VertexPath, const String& _FragmentPath);
	};

}