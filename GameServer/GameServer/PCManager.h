#pragma once
#include <unordered_map>
#include <memory>
#include <mutex>
#include "GameServerDefine.h"
#include "DefineType.h"

class RIOSession;
class PC;

class PCManager
{
private:
	PCManager() = default;
	~PCManager() = default;

public :
	static PCManager& GetInst();

public:
	void InsertPC(RIOSession& session, PCDBID id);

	void ErasePCBySessionId(SessionId sessionId);
	void ErasePCByPCId(PCDBID pcId);

	std::shared_ptr<PC> FindPCBySessionId(SessionId sessionId);
	std::shared_ptr<PC> FindPCByPCId(PCDBID pcId);

private:
	std::unordered_map<SessionId, std::shared_ptr<PC>> sessionIdToPCMap;
	std::mutex sessionIdToPCMapLock;

	std::unordered_map<PCDBID, std::shared_ptr<PC>> pcIdToPCMap;
	std::mutex pcIdToPCMapLock;
};