#pragma once
#include "RIOSession.h"

class GameServerSessionImpl final : public RIOSession
{
public:
	GameServerSessionImpl() = delete;
	explicit GameServerSessionImpl(SOCKET socket, SessionId sessionId, BYTE threadId);
	virtual ~GameServerSessionImpl() = default;

private:

};