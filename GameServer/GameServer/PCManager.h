#pragma once
#include <unordered_map>
#include <memory>
#include <mutex>
#include "GameServerDefine.h"
#include "../../DdingGameServer/RIO_Core/DefineType.h"

class PC;

class PCManager
{
public :
	PCManager& GetInst();

private:
	std::unordered_map<SessionId, std::shared_ptr<PC>> sessionIdToPCMap;
	std::mutex sessionIdToPCMapLock;

	std::unordered_map<PCId, std::shared_ptr<PC>> pcIdToPCMap;
	std::mutex pcIdToPCMapLock;
};