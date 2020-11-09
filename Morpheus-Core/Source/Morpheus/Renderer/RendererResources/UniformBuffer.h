#pragma once

#include "Morpheus/Core/Common.h"

#include <memory>
#include <any>
#include <map>

namespace Morpheus {

	enum class UniformDataType : uint32
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32 UniformDataTypeSize(const UniformDataType& _Type)
	{
		switch (_Type)
		{
			case UniformDataType::Float:		return 4;
			case UniformDataType::Float2:		return 4 * 2;
			case UniformDataType::Float3:		return 4 * 3;
			case UniformDataType::Float4:		return 4 * 4;
			case UniformDataType::Mat3:			return 4 * 9;
			case UniformDataType::Mat4:			return 4 * 16;
			case UniformDataType::Int:			return 4;
			case UniformDataType::Int2:			return 4 * 2;
			case UniformDataType::Int3:			return 4 * 3;
			case UniformDataType::Int4:			return 4 * 4;
			case UniformDataType::Bool:			return 1;
		}
		MORP_CORE_ASSERT(MORP_ERROR, "Unknown UniformDataType!");
		return 0;
	}

	template<typename DataType>
	static DataType GetUniformData(const std::any& _Data)
	{
		return std::any_cast<DataType>(_Data);
	}

	struct BufferElement
	{
		String Name;
		UniformDataType Type;
		uint32 Size;
		uint32 Offset;
		std::any Data;
		bool Normalized;

		BufferElement() {}

		template<typename DataType>
		BufferElement(UniformDataType _Type, const String& _Name, const DataType& _Data, bool _Normalized = false)
			: Name(_Name), Type(_Type), Size(UniformDataTypeSize(_Type)), Offset(0), Normalized(_Normalized)
		{
			Data = _Data;
		}

		uint32 GetComponentCount() const
		{
			switch (Type)
			{
				case UniformDataType::Float:		return 1;
				case UniformDataType::Float2:		return 2;
				case UniformDataType::Float3:		return 3;
				case UniformDataType::Float4:		return 4;
				case UniformDataType::Mat3:			return 9;
				case UniformDataType::Mat4:			return 16;
				case UniformDataType::Int:			return 1;
				case UniformDataType::Int2:			return 2;
				case UniformDataType::Int3:			return 3;
				case UniformDataType::Int4:			return 4;
				case UniformDataType::Bool:			return 1;
			}
			MORP_CORE_ASSERT(MORP_ERROR, "Unknown UniformDataType!");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElement>& _Element)
			: m_Elements(_Element)
		{
			Vector<std::pair<UniformDataType, BufferElement>> PairedElements;
			for (uint32 i = 0; i < m_Elements.size(); i++)
				PairedElements.push_back(std::make_pair(m_Elements[i].Type, m_Elements[i]));
			m_CommonElements = PairedElements;
			CalculateOffsetAndStride();
		}

		inline const uint32& GetStride() { CalculateOffsetAndStride(); return m_Stride; }
		inline const Vector<BufferElement>& GetElements() const { return m_Elements; }
		inline const Vector<std::pair<UniformDataType, BufferElement>>& GetCommons() const { return m_CommonElements; }

		BufferElement& GetElement(const uint32& _Index) { return m_Elements[_Index]; }

		Vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		Vector<BufferElement>::iterator end() { return m_Elements.end(); }
		
		Vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		Vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

		void SetDataAt(const uint32& _Index, const std::any& _Data)
		{ m_Elements[_Index].Data = _Data; }

	private:
		void CalculateOffsetAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}

		}

	private:
		Vector<BufferElement> m_Elements;
		Vector<std::pair<UniformDataType, BufferElement>> m_CommonElements;
		uint32 m_Stride = 0;
	};

	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() = default;

		virtual void SetLayout(const BufferLayout& _Layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;

	public:
		static Ref<UniformBuffer> Create(const BufferLayout& _Layout);
	};

}