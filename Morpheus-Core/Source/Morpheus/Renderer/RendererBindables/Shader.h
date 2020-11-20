#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Mathematics/Mathematics.h"

namespace Morpheus {

	enum class ShaderAttributeType : uint32
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32 ShaderAttributeTypeSize(ShaderAttributeType _Type)
	{
		switch (_Type)
		{
			case ShaderAttributeType::Float:		return 4;
			case ShaderAttributeType::Float2:		return 4 * 2;
			case ShaderAttributeType::Float3:		return 4 * 3;
			case ShaderAttributeType::Float4:		return 4 * 4;
			case ShaderAttributeType::Mat3:			return 4 * 9;
			case ShaderAttributeType::Mat4:			return 4 * 16;
			case ShaderAttributeType::Int:			return 4;
			case ShaderAttributeType::Int2:			return 4 * 2;
			case ShaderAttributeType::Int3:			return 4 * 3;
			case ShaderAttributeType::Int4:			return 4 * 4;
			case ShaderAttributeType::Bool:			return 1;
		}
		MORP_CORE_ASSERT(MORP_ERROR, "Unknown ShaderDataType!");
		return 0;
	}

	struct ShaderAttributeElement
	{
		String Name;
		ShaderAttributeType Type;
		uint32 Size;
		uint32 Offset;

		ShaderAttributeElement() {}
		ShaderAttributeElement(ShaderAttributeType _Type, const String& _Name)
			: Name(_Name), Type(_Type), Size(ShaderAttributeTypeSize(_Type)), Offset(0)
		{
		}

		uint32 GetComponentCount() const
		{
			switch (Type)
			{
				case ShaderAttributeType::Float:		return 1;
				case ShaderAttributeType::Float2:		return 2;
				case ShaderAttributeType::Float3:		return 3;
				case ShaderAttributeType::Float4:		return 4;
				case ShaderAttributeType::Mat3:			return 9;
				case ShaderAttributeType::Mat4:			return 16;
				case ShaderAttributeType::Int:			return 1;
				case ShaderAttributeType::Int2:			return 2;
				case ShaderAttributeType::Int3:			return 3;
				case ShaderAttributeType::Int4:			return 4;
				case ShaderAttributeType::Bool:			return 1;
			}
			MORP_CORE_ASSERT(MORP_ERROR, "Unknown ShaderDataType!");
			return 0;
		}
	};

	class ShaderAttributeLayout
	{
	public:
		ShaderAttributeLayout() {}

		ShaderAttributeLayout(const std::initializer_list<ShaderAttributeElement>& element)
			: m_Elements(element)
		{
			CalculateOffsetAndStride();
		}

		inline uint32 GetStride() const { return m_Stride; }
		inline const Vector<ShaderAttributeElement>& GetElements() const { return m_Elements; }

		Vector<ShaderAttributeElement>::iterator begin() { return m_Elements.begin(); }
		Vector<ShaderAttributeElement>::iterator end() { return m_Elements.end(); }

		Vector<ShaderAttributeElement>::const_iterator begin() const { return m_Elements.begin(); }
		Vector<ShaderAttributeElement>::const_iterator end() const { return m_Elements.end(); }

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
		Vector<ShaderAttributeElement> m_Elements;
		uint32 m_Stride = 0;
	};

	class Shader
	{
	public:
		virtual ~Shader() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual const uint32& GetID() const = 0;

		virtual void SetUniformDescription(const ShaderAttributeLayout& _Layout) = 0;
		virtual void SetLayout(const ShaderAttributeLayout& _Layout) = 0;
		virtual const ShaderAttributeLayout& GetLayout() = 0;

		virtual void SetInt(const String& _Name, const uint32& _Value) = 0;
		virtual void SetFloat(const String& _Name, const float32& _Value) = 0;
		virtual void SetFloat3(const String& _Name, const Vector3& _Value) = 0;
		virtual void SetFloat4(const String& _Name, const Vector4& _Value) = 0;
		virtual void SetMat4(const String& _Name, const Matrix4& _Matrix) = 0;

	public:
		static Ref<Shader> Create(const ShaderAttributeLayout& _Layout, const String& _VertexPath, const String& _FragmentPath);
	};

}