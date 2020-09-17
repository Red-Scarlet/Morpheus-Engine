#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Renderer/RendererCore/Renderpass.h"

namespace Morpheus {

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

	public:
		static Ref<Framebuffer> Create(const Ref<Renderpass>& _Renderpass);

	};

}