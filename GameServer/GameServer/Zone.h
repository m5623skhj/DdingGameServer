#pragma once
#include "GameServerDefine.h"

class Zone
{
public:
	Zone() = delete;
	virtual ~Zone() = default;
	explicit Zone(const ZoneId inZoneId, const Position& inMapMin, const Position& inMapMax);

private:
	const ZoneId zoneId{};
	const Position mapMin;
	const Position mapMax;
};