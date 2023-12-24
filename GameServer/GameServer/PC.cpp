#include "PreCompile.h"
#include "PC.h"
#include "../../DdingGameServer/RIO_Core/RIOSession.h"

PC::PC(RIOSession& inSession, PCDBID inPCId)
	: session(inSession)
	, pcDBID(inPCId)
{
}

void PC::SendPacket(IGameAndClientPacket& packet)
{
	session.SendPacket(packet);
}

void PC::SendPacket(NetBuffer& packet)
{
	session.SendPacket(packet);
}

void PC::SendPacketAndDisconnect(IGameAndClientPacket& packet)
{
	session.SendPacketAndDisconnect(packet);
}

void PC::SendPacketAndDisconnect(NetBuffer& packet)
{
	session.SendPacketAndDisconnect(packet);
}