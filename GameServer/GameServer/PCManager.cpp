#include "PreCompile.h"
#include "PCManager.h"

PCManager& PCManager::GetInst()
{
	static PCManager instance;
	return instance;
}