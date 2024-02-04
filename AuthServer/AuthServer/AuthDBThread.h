#pragma once
#include <string>
#include <thread>
#include <mutex>
#include <list>
#include <queue>

struct DBJobObject
{
	UINT64 sessionId;
	std::string query;
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

	void DBJobHandler();
	std::shared_ptr<DBJobObject> GetDBJobObject();

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