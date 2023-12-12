#include "PreCompile.h"
#include "DBHelper.h"

namespace DBHelper
{
	CSerializationBuf& MakeProcedurePacket(PacketId packetId)
	{
		CSerializationBuf& buffer = *CSerializationBuf::Alloc();
		UINT64 tempDBJobKey = 0;
	
		buffer << packetId;
		buffer << tempDBJobKey;

		return buffer;
	}
}