#include "PreCompile.h"
#include "DungeonBase.h"

DungeonBase::DungeonBase(const ZoneId inZoneId, const DungeonId inDungeonId)
	: dungeonId(inDungeonId)
	, Zone(inZoneId)
{
}