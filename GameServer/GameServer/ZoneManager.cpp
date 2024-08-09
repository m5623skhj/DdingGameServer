#include "PreCompile.h"
#include "GameServerDefine.h"
#include "ZoneManager.h"
#include "Zone.h"

ZoneManager& ZoneManager::GetInst()
{
	static ZoneManager inst;
	return inst;
}
