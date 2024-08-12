#include "PreCompile.h"
#include "DungeonBase.h"

DungeonBase::DungeonBase(const ZoneId inZoneId, const DungeonId inDungeonId, const Position& inMapMin, const Position& inMapMax)
	: dungeonId(inDungeonId)
	, Zone(inZoneId, inMapMin, inMapMax)
{
}