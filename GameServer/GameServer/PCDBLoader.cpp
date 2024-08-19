#include "PreCompile.h"
#include "PC.h"
#include "Logger.h"
#include "PCManager.h"

// PC의 정보를 로드할 때 PC.cpp에 모든 DB 로드 과정이 있으면 복잡하므로,
// 로드를 위한 DB 프로시저들을 해당 파일에서 관리함

bool PC::LoadFromDB()
{
	if (totalDBLoadCount == 0)
	{
		return OnLoadFromDB();
	}

	// Call SP for pc load from db
	{

	}

	return true;
}

bool PC::OnLoadFromDB()
{
	auto log = LogHelper::MakeLogObject<ServerLog>();
	log->logString = "PC " + pcDBID;
	log->logString += " is load completed from db";

	Logger::GetInstance().WriteLog(log);

	return FinalizeLoadFromDB();
}

bool PC::FinalizeLoadFromDB()
{
	if (totalDBFinalizeLoadCount == 0)
	{
		return OnFinalizeLoadFromDB();
	}

	// Call SP for pc finalize load from db
	{

	}
	
	return true;
}

bool PC::OnFinalizeLoadFromDB()
{
	auto log = LogHelper::MakeLogObject<ServerLog>();
	log->logString = "PC " + pcDBID;
	log->logString += " is finalize load completed from db";

	Logger::GetInstance().WriteLog(log);

	return true;
}

bool PC::OnDBLoadCompleted()
{
	PCManager::GetInst().InsertPC(session, pcDBID);

	return true;
}