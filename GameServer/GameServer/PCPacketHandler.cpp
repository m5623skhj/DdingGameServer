#include "PreCompile.h"
#include "PacketManager.h"
#include "PC.h"
#include "PCManager.h"

bool PacketManager::HandlePacket(RIOSession& session, Ping& packet)
{
	auto pc = PCManager::GetInst().FindPCBySessionId(session.GetSessionId());
	if (pc == nullptr)
	{
		return false;
	}

	pc->UpdateByPing();

	Pong pong;
	pc->SendPacket(pong);

	return true;
}
