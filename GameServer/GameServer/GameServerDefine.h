#pragma once

#include <Windows.h>

#define INVALID_PC_ID 0
#define INVALID_DATA_ID -1
#define INVLAID_GAME_OBJECT_ID 0
#define INVALID_ZONE_ID 0

using PCDBID = UINT64;
using GameObjectId = UINT64;
using ZoneDataId = int;
using DungeonDataId = int;
using NPCDataId = unsigned int;

struct Position
{
	UINT x = 0;
	UINT y = 0;
};
