#include "PreCompile.h"
#include "GameServerDefine.h"
#include "DungeonManager.h"

DungeonManager& DungeonManager::GetInst()
{
	static DungeonManager inst;
	return inst;
}