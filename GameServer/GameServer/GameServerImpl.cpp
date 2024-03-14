#include "PreCompile.h"
#include "GameServerImpl.h"
#include "GameServerSessionImpl.h"

std::shared_ptr<RIOSession> GameServerImpl::GetNewSession(SOCKET enteredClientSocket, BYTE threadId)
{
	SessionId newSessionId = InterlockedIncrement(&nextSessionId);
	if (newSessionId == INVALID_SESSION_ID)
	{
		return nullptr;
	}

	return std::make_shared<GameServerSessionImpl>(enteredClientSocket, newSessionId, threadId);
}