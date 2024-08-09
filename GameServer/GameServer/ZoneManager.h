#pragma once
#include <unordered_map>

class Zone;

class ZoneManager
{
private:
	ZoneManager() = default;
	~ZoneManager() = default;

public:
	static ZoneManager& GetInst();

private:
	std::unordered_map<ZoneId, std::shared_ptr<Zone>> zoneMap;
};