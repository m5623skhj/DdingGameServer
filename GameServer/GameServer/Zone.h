#pragma once
#include "GameServerDefine.h"

class Zone
{
public:
	Zone() = delete;
	virtual ~Zone() = default;
	explicit Zone(const ZoneDataId inZoneId, const Position& inMapMin, const Position& inMapMax);

private:
	const ZoneDataId zoneId{};
	const Position mapMin;
	const Position mapMax;
};