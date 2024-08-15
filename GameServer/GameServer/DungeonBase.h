#pragma once
#include "Zone.h"
#include "DungeonData.h"

class DungeonBase : public Zone
{
public:
	DungeonBase() = delete;
	virtual ~DungeonBase() = default;
	explicit DungeonBase(const ZoneDataId inZoneId, const DungeonDataId inDungeonId, const Position& inMapMin, const Position& inMapMax);

private:
	DungeonDataId dungeonId{};
	std::string dungeonName{};
	unsigned short dungeonLevel{};
};