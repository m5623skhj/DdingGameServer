#include "PreCompile.h"
#include "PacketManager.h"
#include "RIOSession.h"
#include "Protocol.h"

bool PacketManager::HandlePacket(RIOSession& session, Ping& packet)
{
	Pong pong;
	session.SendPacket(pong);

	return true;
}
