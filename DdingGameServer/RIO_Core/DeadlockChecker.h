#pragma once

#include <map>
#include <mutex>
#include <functional>

using CheckSameCount = WORD;
using UpdateCountGetter = std::function<UINT64()>;

class DeadlockChecker
{
private:
	DeadlockChecker();
	~DeadlockChecker();

	DeadlockChecker(const DeadlockChecker&) = delete;
	DeadlockChecker& operator=(const DeadlockChecker&) = delete;

public:
	static DeadlockChecker& GetInstance();

#pragma region ThreadSatateCheck
public:
	bool RegisterCheckThread(const std::thread::id& threadId, const UpdateCountGetter& countGetterFunctor);
	void DeregisteredCheckThread(const std::thread::id& threadId);

private:
	void CheckDeadlockThread();
	
private:
	std::thread checkerThread;

	std::map<std::thread::id, const UpdateCountGetter&> checkerMap;
	mutable std::mutex lock;
		
	// 20 seconds
	CheckSameCount maxCheckCount = 20000;
	// 2 seconds
	const DWORD checkSleepTime = 2000;
#pragma endregion ThreadSatateCheck
};