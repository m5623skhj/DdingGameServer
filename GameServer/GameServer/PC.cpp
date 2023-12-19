#include "PreCompile.h"
#include "PC.h"
#include "../../DdingGameServer/RIO_Core/RIOSession.h"

PC::PC(SessionId inSessionId, PCId inPCId)
	: sessionId(inSessionId)
	, pcId(inPCId)
{
}