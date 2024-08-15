#include "PreCompile.h"
#include "Zone.h"

Zone::Zone(const ZoneDataId inZoneId, const Position& inMapMin, const Position& inMapMax)
	: zoneId(inZoneId)
	, mapMin(inMapMin)
	, mapMax(inMapMax)
{
}