#include "Morppch.h"
#include "EngineMessageSystem.h"

namespace Morpheus {

	uint32 EngineMessageSystem::Size() const
	{
		return sizeof(EngineMessage) + Body.size();
	}

	void EngineMessageSystem::SetHeader(const EngineMessage& _Header)
	{
		Header.ID = _Header;
	}

	Ostream& operator<<(Ostream& _Os, const EngineMessageSystem& _Message)
	{
		_Os << "ID: " << (uint32)_Message.Header.ID << " Size: " << (uint32)_Message.Header.Size; return _Os;
	}

	bool operator==(EngineMessageSystem& _Message, const EngineMessage& _Header)
	{
		return _Message.Header.ID == _Header;
	}

}
