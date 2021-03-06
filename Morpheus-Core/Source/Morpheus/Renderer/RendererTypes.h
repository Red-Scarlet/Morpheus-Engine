#pragma once

#include "Morpheus/Core/Common.h"

namespace Morpheus {

	enum class PipelineAttributeType : uint32
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	enum class RendererImageFormat
	{
		FORMAT_DEFAULT,

		FORMAT_R8G8B8_SINT,
		FORMAT_R16G16B16_SINT,
		FORMAT_R32G32B32_SINT,
		FORMAT_R8G8B8A8_SINT,
		FORMAT_R16G16B16A16_SINT,
		FORMAT_R32G32B32A32_SINT,

		FORMAT_D32_SFLOAT,
		FORMAT_R16G16B16_SFLOAT,
		FORMAT_R32G32B32_SFLOAT,
		FORMAT_R16G16B16A16_SFLOAT,
		FORMAT_R32G32B32A32_SFLOAT,

		FORMAT_R8G8B8_UNORM,
		FORMAT_R16G16B16_UNORM,
		FORMAT_R8G8B8A8_UNORM,
		FORMAT_R16G16B16A16_UNORM,

		FORMAT_R8G8B8_SNORM,
		FORMAT_R16G16B16_SNORM,
		FORMAT_R8G8B8A8_SNORM,
		FORMAT_R16G16B16A16_SNORM
	};

	enum class PipelineDescriptorType
	{
		DESCRIPTOR_TYPE_SAMPLER,
		DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
		DESCRIPTOR_TYPE_SAMPLED_IMAGE,
		DESCRIPTOR_TYPE_STORAGE_IMAGE,
		DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
		DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,
		DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		DESCRIPTOR_TYPE_STORAGE_BUFFER,
		DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
		DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC,
		DESCRIPTOR_TYPE_INPUT_ATTACHMENT
	};

	enum class PipelineDescriptorStage
	{
		DESCRIPTOR_VERTEX,
		DESCRIPTOR_FRAGMENT
	};

	enum class RendererImageLayout
	{
		USAGE_UNDEFINED_ATTACHMENT,
		USAGE_PRESENT_ATTACHMENT,
		USAGE_COLOR_ATTACHMENT,
		USAGE_DEPTH_STENCIL_ATTACHMENT,

		USAGE_DEPTH_STENCIL_READ_ATTACHMENT,
		USAGE_SHADER_READ_ATTACHMENT,
	};

	enum class RendererImageUsage
	{
		USAGE_COLOR_BIT,
		USAGE_DEPTH_STENCIL_BIT
	};

	struct RendererBufferData
	{
	public:
		void* Data;
		uint32 Size;

		RendererBufferData() = default;
		RendererBufferData(void* _Data, const uint32& _Size)
			: Data(_Data), Size(_Size) 
		{
		}
	};


}