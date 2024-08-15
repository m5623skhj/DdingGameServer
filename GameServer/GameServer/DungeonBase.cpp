#include "PreCompile.h"
#include "DungeonBase.h"

DungeonBase::DungeonBase(const ZoneDataId inZoneId, const DungeonDataId inDungeonId, const Position& inMapMin, const Position& inMapMax)
	: dungeonId(inDungeonId)
	, Zone(inZoneId, inMapMin, inMapMax)
{
}