#pragma once
#include "GameServerDefine.h"

class Zone
{
public:
	Zone() = delete;
	virtual ~Zone() = default;
	explicit Zone(const ZoneId inZoneId);

private:
	const ZoneId zoneId{};
};