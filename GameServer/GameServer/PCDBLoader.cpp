#include "PreCompile.h"
#include "PC.h"
#include "Logger.h"
#include "PCManager.h"

// PC�� ������ �ε��� �� PC.cpp�� ��� DB �ε� ������ ������ �����ϹǷ�,
// �ε带 ���� DB ���ν������� �ش� ���Ͽ��� ������

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