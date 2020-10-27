#pragma once

#include "Common.h"

namespace Morpheus {

	enum class EngineMessage : uint32
	{
		UNIFORM_BUFFER
	};

	struct EngineMessageSystem
	{
	public:
		uint32 Size() const;
		void SetHeader(const EngineMessage& _Header);

		friend Ostream& operator<< (Ostream& _Os, const EngineMessageSystem& _Message);
		friend bool operator== (EngineMessageSystem& _Message, const EngineMessage& _Header);

		template<typename DataType>
		friend EngineMessageSystem& operator<< (EngineMessageSystem& _Message, const DataType& _Data);

		template<typename DataType>
		friend EngineMessageSystem& operator>> (EngineMessageSystem& _Message, DataType& _Data);

		const Vector<uint8>& GetData() { return Body; }

	private:
		Vector<uint8> Body;
		struct {
			EngineMessage ID;
			uint32 Size;
		} Header;

	};

	template<typename DataType>
	EngineMessageSystem& operator<<(EngineMessageSystem& _Message, const DataType& _Data)
	{
		static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

		size_t i = _Message.Body.size();
		_Message.Body.resize(_Message.Body.size() + sizeof(DataType));
		std::memcpy(_Message.Body.data() + i, &_Data, sizeof(DataType));

		_Message.Header.Size = _Message.Size();
		return _Message;
	}

	template<typename DataType>
	EngineMessageSystem& operator>>(EngineMessageSystem& _Message, DataType& _Data)
	{
		static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into vector");

		size_t i = _Message.Body.size() - sizeof(DataType);
		std::memcpy(&_Data, _Message.Body.data() + i, sizeof(DataType));
		_Message.Body.resize(i);

		_Message.Header.Size = _Message.Size();
		return _Message;
	}

}