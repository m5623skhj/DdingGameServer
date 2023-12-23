#pragma once
#include "Creature.h"
#include "GameServerDefine.h"
#include <memory>
#include "../../DdingGameServer/RIO_Core/DefineType.h"
#include "../../DdingGameServer/RIO_Core/RIOSession.h"

class PC : public Creature
{
public:
	PC() = delete;
	virtual ~PC() = default;
	explicit PC(RIOSession& inSession, PCId inPCId);

public:
	void SendPacket(IGameAndClientPacket& packet);
	void SendPacket(NetBuffer& packet);
	void SendPacketAndDisconnect(IGameAndClientPacket& packet);
	void SendPacketAndDisconnect(NetBuffer& packet);

public:
	SessionId GetSessionId() const { return pcId; }
	PCId GetPCId() const { return session.GetSessionId(); }

private:
	PCId pcId;

private:
	RIOSession& session;
};