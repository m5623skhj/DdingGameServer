#include "PreCompile.h"
#include "AuthDBThread.h"

#define DB_JOB_HANDLE  WAIT_OBJECT_0
#define STOP_HANDLE WAIT_OBJECT_0 + 1

#define DB_JOB_EVENT_HANDLE dbJobEventHandles[0]
#define STOP_EVENT_HANDLE dbJobEventHandles[1]

bool AuthDBThreadManager::StartThreads(BYTE numOfThreads)
{
	if (isStarted == true)
	{
		std::cout << "Auth db threads already started" << std::endl;
		return false;
	}

	DB_JOB_EVENT_HANDLE = CreateEvent(NULL, FALSE, FALSE, NULL);
	STOP_EVENT_HANDLE = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (DB_JOB_EVENT_HANDLE == NULL || STOP_EVENT_HANDLE == NULL)
	{
		std::cout << "Logger event handle is invalid" << std::endl;
		g_Dump.Crash();
	}

	for (BYTE i = 0; i < numOfThreads; ++i)
	{
		workerThreads.emplace_back([this]() { this->Worker(); });
	}

	isStarted = true;
	return true;
}

void AuthDBThreadManager::StopThreads()
{
	SetEvent(STOP_EVENT_HANDLE);
	for (auto& thread : workerThreads)
	{
		thread.join();
	}

	CloseHandle(STOP_EVENT_HANDLE);
	CloseHandle(DB_JOB_EVENT_HANDLE);
}

void AuthDBThreadManager::Worker()
{
	while (true)
	{
		auto result = WaitForMultipleObjects(2, dbJobEventHandles, FALSE, INFINITE);
		if (result == DB_JOB_HANDLE)
		{
			DBJobHandler();
		}
		else if (result == STOP_HANDLE)
		{
			SetEvent(STOP_EVENT_HANDLE);
			break;
		}
		else
		{
			g_Dump.Crash();
		}
	}
}

void AuthDBThreadManager::DBJobHandler()
{
	while (true)
	{
		if (auto job = GetDBJobObject())
		{
			// call sp procedure
		}
		else
		{
			return;
		}
	}
}

std::shared_ptr<DBJobObject> AuthDBThreadManager::GetDBJobObject()
{
	std::lock_guard<std::mutex> lock(dbJobListLock);
	if (dbJobList.size() == 0)
	{
		return nullptr;
	}

	auto job = dbJobList.front();
	dbJobList.pop();

	return job;
}