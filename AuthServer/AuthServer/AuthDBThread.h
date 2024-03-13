#pragma once
#include <string>
#include <thread>
#include <mutex>
#include <list>
#include <queue>
#include "Type.h"
#include "AuthQuery.h"
#include <functional>
#include <unordered_map>

struct DBConnection;

struct DBJobObject
{
	ClientType clientType;
	UINT64 sessionId;
	QueryObject queryObject;
};

using DBJobHandler = std::function<void(const DBJobObject&)>;

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
	void DBJobResultHandler(DBConnection& conn, const DBJobObject& jobObject);

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

#pragma region NeedDBResultHandler
private:
	void NotNeedDBResultHandler();
	void NeedDBResultHandler(DBConnection& conn, const DBJobObject& jobObject);
	DBJobHandler GetDBJobHandler(QueryType queryType);

private:
	std::unordered_map<QueryType, DBJobHandler> dbJobHandlerMap;
#pragma endregion NeedDBResultHandler

#pragma region DeclareResultHandler
private:

#pragma endregion DeclareResultHandler
};