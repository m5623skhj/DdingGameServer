#include "PreCompile.h"
#include "GameServerDefine.h"
#include "ZoneManager.h"
#include "Zone.h"
#include "Logger.h"

ZoneManager& ZoneManager::GetInst()
{
	static ZoneManager inst;
	return inst;
}

void ZoneManager::InsertZone(const ZoneDataId inZoneId, std::shared_ptr<Zone> inZone)
{
	std::unique_lock lock(zoneMapLock);

	auto log = LogHelper::MakeLogObject<ServerLog>();
	auto result = zoneMap.insert({ inZoneId, inZone });
	if (result.second == false)
	{
		log->logString = "Duplicate zone ZoneId " + inZoneId;
	}
	else
	{
		log->logString = "Zone inserted ZoneId " + inZoneId;
	}

	Logger::GetInstance().WriteLog(log);
}

void ZoneManager::DeleteZone(const ZoneDataId inZoneId)
{
	std::unique_lock lock(zoneMapLock);

	auto log = LogHelper::MakeLogObject<ServerLog>();
	auto result = zoneMap.erase(inZoneId);
	if (result == 0)
	{
		log->logString = "Zone delete failed. ZoneId " + inZoneId;
		Logger::GetInstance().WriteLog(log);
	}
	else
	{
		log->logString = "Zone delete successed. ZoneId " + inZoneId;
		Logger::GetInstance().WriteLog(log);
	}

	Logger::GetInstance().WriteLog(log);
}

std::shared_ptr<Zone> ZoneManager::GetZone(const ZoneDataId findZoneId)
{
	std::shared_lock lock(zoneMapLock);

	auto iter = zoneMap.find(findZoneId);
	if (iter == zoneMap.end())
	{
		return nullptr;
	}

	return iter->second;
}