#pragma once
#include <unordered_map>
#include <shared_mutex>

class Zone;

class ZoneManager
{
private:
	ZoneManager() = default;
	~ZoneManager() = default;

public:
	static ZoneManager& GetInst();
	void InsertZone(const ZoneDataId inZoneId, std::shared_ptr<Zone> inZone);
	void DeleteZone(const ZoneDataId inZoneId);
	std::shared_ptr<Zone> GetZone(const ZoneDataId findZoneId);

private:
	std::unordered_map<ZoneDataId, std::shared_ptr<Zone>> zoneMap;
	std::shared_mutex zoneMapLock;
};