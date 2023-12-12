#pragma once
#include <thread>

class IOCPTimer
{
public:
	IOCPTimer() = default;
	virtual ~IOCPTimer() = default;

public:
	virtual void StartTimer() = 0;
	virtual void OnTimer() = 0;
	virtual void ResumeTimer() = 0;

protected:
	void SetTick(UINT64 tick) { timerTick = tick; }

private:
	UINT64 timerTick = 0;
};

class IOCPTimerManager
{
public:
	IOCPTimerManager();
	~IOCPTimerManager();

public:
	void RunTimerThread();

private:
	std::thread timerThread;
};