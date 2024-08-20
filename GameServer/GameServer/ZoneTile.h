#pragma once
#include "GameServerDefine.h"

class ZoneTile
{
public:
	ZoneTile() = delete;
	~ZoneTile() = default;
	explicit ZoneTile(const Position& inTileSize);

private:
	Position tileSize;
};