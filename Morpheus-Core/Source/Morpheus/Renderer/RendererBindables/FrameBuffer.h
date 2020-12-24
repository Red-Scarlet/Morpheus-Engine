#pragma once

#include "Morpheus/Core/Common.h"

#include "Morpheus/Renderer/RendererResources/Renderpass.h"

namespace Morpheus {

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual const uint32& GetID() const = 0;
		virtual void* GetImage() = 0;

	public:
		static Ref<FrameBuffer> Create(const Ref<Renderpass>& _Renderpass);
	};

}