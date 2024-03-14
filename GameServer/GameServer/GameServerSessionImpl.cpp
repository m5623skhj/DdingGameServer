#include "PreCompile.h"
#include "GameServerSessionImpl.h"

GameServerSessionImpl::GameServerSessionImpl(SOCKET socket, SessionId sessionId, BYTE threadId)
	: RIOSession(socket, sessionId, threadId)
{

}