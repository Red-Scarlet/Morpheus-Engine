#pragma once

#include "Morpheus/Core/Common.h"
#include "Morpheus/Mathematics/Mathematics.h"

namespace Morpheus {

	enum class UniformAttributeType
	{ None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool };

	static uint32 UniformAttributeTypeSize(UniformAttributeType _Type)
	{
		switch (_Type)
		{
			case UniformAttributeType::Float:			return 4;
			case UniformAttributeType::Float2:			return 4 * 2;
			case UniformAttributeType::Float3:			return 4 * 3;
			case UniformAttributeType::Float4:			return 4 * 4;
			case UniformAttributeType::Mat3:			return 4 * 9;
			case UniformAttributeType::Mat4:			return 4 * 16;
			case UniformAttributeType::Int:				return 4;
			case UniformAttributeType::Int2:			return 4 * 2;
			case UniformAttributeType::Int3:			return 4 * 3;
			case UniformAttributeType::Int4:			return 4 * 4;
			case UniformAttributeType::Bool:			return 1;
		}
		MORP_CORE_ASSERT(MORP_ERROR, "Unknown UniformDataType!");
		return 0;
	}

	struct UniformAttributeElement
	{
		String Name;
		UniformAttributeType Type;
		uint32 Size;
		uint32 Offset;

		UniformAttributeElement() {}
		UniformAttributeElement(const UniformAttributeType& _Type, const String& _Name)
			: Name(_Name), Type(_Type), Size(UniformAttributeTypeSize(_Type)), Offset(0)
		{
		}

		uint32 GetComponentCount() const
		{
			switch (Type)
			{
				case UniformAttributeType::Float:			return 1;
				case UniformAttributeType::Float2:			return 2;
				case UniformAttributeType::Float3:			return 3;
				case UniformAttributeType::Float4:			return 4;
				case UniformAttributeType::Mat3:			return 9;
				case UniformAttributeType::Mat4:			return 16;
				case UniformAttributeType::Int:				return 1;
				case UniformAttributeType::Int2:			return 2;
				case UniformAttributeType::Int3:			return 3;
				case UniformAttributeType::Int4:			return 4;
				case UniformAttributeType::Bool:			return 1;
			}
			MORP_CORE_ASSERT(MORP_ERROR, "Unknown ShaderDataType!");
			return 0;
		}
	};

	class UniformAttributeLayout
	{
	public:
		UniformAttributeLayout() {}

		UniformAttributeLayout(const std::initializer_list<UniformAttributeElement>& element)
			: m_Elements(element)
		{
			CalculateOffsetAndStride();
		}

		inline const uint32& GetStride() const { return m_Stride; }
		inline const Vector<UniformAttributeElement>& GetElements() const { return m_Elements; }

		Vector<UniformAttributeElement>::iterator begin() { return m_Elements.begin(); }
		Vector<UniformAttributeElement>::iterator end() { return m_Elements.end(); }

		Vector<UniformAttributeElement>::const_iterator begin() const { return m_Elements.begin(); }
		Vector<UniformAttributeElement>::const_iterator end() const { return m_Elements.end(); }

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
		Vector<UniformAttributeElement> m_Elements;
		uint32 m_Stride = 0;
	};

	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() = default;
		virtual const uint32& GetID() const = 0;

		virtual void SetInt(const String& _Name, const uint32& _Value) = 0;
		virtual void SetFloat(const String& _Name, const float32& _Value) = 0;
		virtual void SetFloat3(const String& _Name, const Vector3& _Value) = 0;
		virtual void SetFloat4(const String& _Name, const Vector4& _Value) = 0;
		virtual void SetMat4(const String& _Name, const Matrix4& _Matrix) = 0;

	public:
		static Ref<UniformBuffer> Create(const UniformAttributeLayout& _Layout);

	};

}