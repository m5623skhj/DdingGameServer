#pragma once
#include <string>
#include <thread>
#include <mutex>
#include <list>
#include <queue>
#include "Type.h"

struct DBConnection;

struct DBJobObject
{
	ClientType clientType;
	UINT64 sessionId;
	std::wstring query;
};

class AuthDBThreadManager
{
private:
	AuthDBThreadManager() = default;
	~AuthDBThreadManager() = default;
	AuthDBThreadManager(const AuthDBThreadManager& other) = delete;
	AuthDBThreadManager& operator=(const AuthDBThreadManager& other) = delete;

public:
	static AuthDBThreadManager& GetInst()
	{
		static AuthDBThreadManager instance;
		return instance;
	}

public:
	bool StartThreads(BYTE numOfThreads);
	void StopThreads();

private:
	void Worker();

	void DBJobHandle();
	std::shared_ptr<DBJobObject> GetDBJobObject();
	void CallSP(std::shared_ptr<DBJobObject> dbJob);
	void DBJobResultHandler(DBConnection& conn);

public:
	void InsertDBJob(std::shared_ptr<DBJobObject> dbJob);

private:
	bool isStarted = false;

private:
	// 0. DBJobHandle
	// 1. StopHandle
	HANDLE dbJobEventHandles[2];
	std::list<std::thread> workerThreads;

	std::queue<std::shared_ptr<DBJobObject>> dbJobList;
	std::mutex dbJobListLock;
};