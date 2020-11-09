#pragma once

#include "Morpheus/Core/Common.h"

#include "Morpheus/Renderer/RendererResources/UniformBuffer.h"

namespace Morpheus {

	enum class VertexAttributeType : uint32
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32 VertexAttributeTypeSize(VertexAttributeType _Type)
	{
		switch (_Type)
		{
		case VertexAttributeType::Float:		return 4;
		case VertexAttributeType::Float2:		return 4 * 2;
		case VertexAttributeType::Float3:		return 4 * 3;
		case VertexAttributeType::Float4:		return 4 * 4;
		case VertexAttributeType::Mat3:			return 4 * 9;
		case VertexAttributeType::Mat4:			return 4 * 16;
		case VertexAttributeType::Int:			return 4;
		case VertexAttributeType::Int2:			return 4 * 2;
		case VertexAttributeType::Int3:			return 4 * 3;
		case VertexAttributeType::Int4:			return 4 * 4;
		case VertexAttributeType::Bool:			return 1;
		}
		MORP_CORE_ASSERT(MORP_ERROR, "Unknown ShaderDataType!");
		return 0;
	}

	struct VertexAttributeElement
	{
		String Name;
		VertexAttributeType Type;
		uint32 Size;
		uint32 Offset;

		VertexAttributeElement() {}
		VertexAttributeElement(VertexAttributeType _Type, const String& _Name)
			: Name(_Name), Type(_Type), Size(VertexAttributeTypeSize(_Type)), Offset(0)
		{
		}

		uint32 GetComponentCount() const
		{
			switch (Type)
			{
			case VertexAttributeType::Float:		return 1;
			case VertexAttributeType::Float2:		return 2;
			case VertexAttributeType::Float3:		return 3;
			case VertexAttributeType::Float4:		return 4;
			case VertexAttributeType::Mat3:			return 9;
			case VertexAttributeType::Mat4:			return 16;
			case VertexAttributeType::Int:			return 1;
			case VertexAttributeType::Int2:			return 2;
			case VertexAttributeType::Int3:			return 3;
			case VertexAttributeType::Int4:			return 4;
			case VertexAttributeType::Bool:			return 1;
			}
			MORP_CORE_ASSERT(MORP_ERROR, "Unknown ShaderDataType!");
			return 0;
		}
	};

	class VertexAttributeLayout
	{
	public:
		VertexAttributeLayout() {}

		VertexAttributeLayout(const std::initializer_list<VertexAttributeElement>& element)
			: m_Elements(element)
		{
			CalculateOffsetAndStride();
		}

		inline uint32 GetStride() const { return m_Stride; }
		inline const Vector<VertexAttributeElement>& GetElements() const { return m_Elements; }

		Vector<VertexAttributeElement>::iterator begin() { return m_Elements.begin(); }
		Vector<VertexAttributeElement>::iterator end() { return m_Elements.end(); }

		Vector<VertexAttributeElement>::const_iterator begin() const { return m_Elements.begin(); }
		Vector<VertexAttributeElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		void CalculateOffsetAndStride()
		{
			uint32 offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}

	private:
		Vector<VertexAttributeElement> m_Elements;
		uint32 m_Stride = 0;
	};

	struct ShaderInfo
	{
	public:
		String VertexPath;
		String FragmentPath;
	};

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() = 0;

		virtual void SetLayout(const VertexAttributeLayout& _Layout) = 0;
		virtual const VertexAttributeLayout& GetLayout() const = 0;

	public:
		static Ref<Shader> Create(const VertexAttributeLayout& _Layout, const ShaderInfo& _ShaderPath);
	};

}