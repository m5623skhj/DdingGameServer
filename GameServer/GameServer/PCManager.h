#pragma once
#include <unordered_map>
#include <memory>
#include <mutex>
#include "GameServerDefine.h"
#include "../../DdingGameServer/RIO_Core/DefineType.h"

class PC;

class PCManager
{
private:
	PCManager() = default;
	~PCManager() = default;

public :
	PCManager& GetInst();

public:
	// TODO :
	// SessionId나 PCId 둘 중 하나로만 Add, Delete 해도 두 개 모두에 들어갔으면 좋겠음
	void AddPCBySessionId(SessionId sessionId, std::shared_ptr<PC> pc);
	void AddPCByPCId(PCId pcId, std::shared_ptr<PC> pc);

	void DeletePCBySessionId(SessionId sessionId);
	void DeletePCByPCId(PCId pcId);

	std::shared_ptr<PC> FindPCBySessionId(SessionId sessionId);
	std::shared_ptr<PC> FindPCByPCId(PCId pcId);

private:
	std::unordered_map<SessionId, std::shared_ptr<PC>> sessionIdToPCMap;
	std::mutex sessionIdToPCMapLock;

	std::unordered_map<PCId, std::shared_ptr<PC>> pcIdToPCMap;
	std::mutex pcIdToPCMapLock;
};