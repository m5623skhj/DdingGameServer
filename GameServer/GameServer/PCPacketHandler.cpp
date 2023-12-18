#include "PreCompile.h"
#include "../../DdingGameServer/RIO_Core/PacketManager.h"
#include "../../DdingGameServer/RIO_Core/RIOSession.h"
#include "PC.h"

bool PacketManager::HandlePacket(RIOSession& session, Ping& packet)
{
	Pong pong;
	//session.SendPacket(pong);

	return true;
}
