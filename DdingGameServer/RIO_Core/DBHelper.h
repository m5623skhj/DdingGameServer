#pragma once
#include "LanServerSerializeBuf.h"
#include "Protocol.h"

namespace DBHelper
{
	CSerializationBuf& MakeProcedurePacket(PacketId packetId);
}
