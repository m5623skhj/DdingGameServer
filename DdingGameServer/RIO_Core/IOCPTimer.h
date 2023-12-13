#pragma once
#include <thread>
#include <mutex>
#include <map>

class IOCPTimer
{
public:
	IOCPTimer();
	virtual ~IOCPTimer() = default;

public:
	virtual void StartTimer() = 0;
	virtual void OnTimer() = 0;

protected:
	void ResumeTimer();
	
	void SetNextWakeUpTick(UINT64 nextWakeUpTime) { wakeUpTime = nextWakeUpTime; }

public:
	UINT64 GetTimerId() { return timerId; }

private:
	UINT64 timerId = 0;
	UINT64 wakeUpTime = 0;
	OVERLAPPED overlapped;
};

class IOCPTimerManager
{
private:
	IOCPTimerManager();
	~IOCPTimerManager();

public:
	static IOCPTimerManager& GetInst()
	{
		static IOCPTimerManager instance;
		return instance;
	}

public:
	UINT64 GetTimerId() { return nextTimerId++; }
	void RunTimerThread();

public:
	void RegisterTimer(std::shared_ptr<IOCPTimer> timer);
	void ResumeTimer(UINT64 timerId, LPOVERLAPPED timerOverlapped);
	void StopTimer(UINT64 timerId);
	void StopTimerManager();

private:
	HANDLE iocpHandle;
	std::thread timerThread;
	std::mutex timerMapLock;
	std::map<UINT64, std::shared_ptr<IOCPTimer>> timerMap;

	bool isValid = true;
	bool isRunning = false;

	std::atomic<UINT64> nextTimerId = 1;
};