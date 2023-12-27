#include "PreCompile.h"
#include "PC.h"

// PC의 정보를 로드할 때 PC.cpp에 모든 DB 로드 과정이 있으면 복잡하므로,
// 로드를 위한 DB 프로시저들을 해당 파일에서 관리함

bool PC::LoadFromDB()
{
	return true;
}

bool PC::OnLoadFromDB()
{
	return true;
}

void PC::FinalizeOnLoadFromDB()
{
}