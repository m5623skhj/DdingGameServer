#pragma once
#include <string>
#include <thread>
#include <mutex>
#include <list>

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

private:
	bool isStarted = false;

private:
	HANDLE iocpHandle;
	std::list<std::thread> workerThreads;
};