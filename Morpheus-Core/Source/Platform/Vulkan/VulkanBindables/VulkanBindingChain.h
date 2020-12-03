#pragma once

#include "Morpheus/Core/Common.h"

namespace Morpheus {

	class VulkanBindingChain
	{
	public:
		VulkanBindingChain() = default;

		void Reset()
		{
			m_FrameBuffer = uint32_max;
			m_Shader = uint32_max;
			m_VertexArray = uint32_max;
		}

		void SetFrameBuffer(const uint32& _ID) { m_FrameBuffer = _ID; }
		void SetShader(const uint32& _ID) { m_Shader = _ID; }
		void SetVertexArray(const uint32& _ID) { m_VertexArray = _ID; }

		const uint32& GetFrameBufferID() { return m_FrameBuffer; }
		const uint32& GetShaderID() { return m_Shader; }
		const uint32& GetVertexArrayID() { return m_VertexArray; }

	private:
		uint32 m_FrameBuffer;
		uint32 m_Shader;
		uint32 m_VertexArray;

	};

}