#pragma once
#include "Zone.h"

class DungeonBase : public Zone
{
public:
	DungeonBase() = delete;
	virtual ~DungeonBase() = default;
	explicit DungeonBase(const ZoneId inZoneId, const DungeonId inDungeonId, const Position& inMapMin, const Position& inMapMax);

private:
	DungeonId dungeonId{};
};