#pragma once
#include "GameServerDefine.h"
#include <memory>
#include "../../DdingGameServer/RIO_Core/DefineType.h"

class RIOSession;

class PC
{
public:
	PC() = delete;
	~PC() = default;
	explicit PC(SessionId inSessionId, PCId inPCId);

public:
	SessionId GetSessionId() const { return pcId; }
	PCId GetPCId() const { return sessionId; }

private:
	PCId pcId;
	SessionId sessionId;
};