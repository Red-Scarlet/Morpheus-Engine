#pragma once

#include "Morpheus/Core/Common.h"

#include "UniformBuffer.h"

namespace Morpheus {

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() = 0;

	public:
		static Ref<Shader> Create(const String& _VertexPath, const String& _FragmentPath);
	};

}