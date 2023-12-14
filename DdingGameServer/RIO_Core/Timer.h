#pragma once
#include <thread>
#include <mutex>
#include <list>
#include <unordered_map>

/// <summary>
/// Timer를 ResumeTimer() 시키기 이전에 
/// 반드시 TimerManager에 RegisterTimer()로 해당 타이머를 등록시켜야 하며,
/// SetNextWakeUpTime()으로 다음 깨어나는 시간을 설정해 놓아야 정상 동작함
/// </summary>
class Timer
{
public:
	Timer();
	virtual ~Timer() = default;

public:
	virtual void OnTimer() = 0;

public:
	void ResumeTimer();

public:
	long long GetNextWakeUpTime() { return wakeUpTime; }
	void SetNextWakeUpTime(long long nextWakeUpTime) { wakeUpTime = nextWakeUpTime; }

public:
	UINT64 GetTimerId() { return timerId; }

private:
	UINT64 timerId = 0;
	long long wakeUpTime = LLONG_MAX;
};

class TimerManager
{
private:
	TimerManager();
	~TimerManager();

public:
	static TimerManager& GetInst()
	{
		static TimerManager instance;
		return instance;
	}

public:
	UINT64 GetTimerId() { return nextTimerId++; }
	void RunTimerThread();

private:
	void GetExpiredTimer(std::list<std::shared_ptr<Timer>>& outList);

public:
	void RegisterTimer(std::shared_ptr<Timer> timer);
	void DeregisterTimer(UINT64 timerId);
	bool ResumeTimer(UINT64 timerId);
	void StopTimer(UINT64 timerId);
	void StopTimerManager();

private:
	std::thread timerThread;
	HANDLE threadStopEventHandle;

	std::list<std::shared_ptr<Timer>> timerList;
	std::mutex timerListLock;

	std::unordered_map<UINT64, std::shared_ptr<Timer>> registeredTimerMap;
	std::mutex registeredTimerMapLock;

	bool isRunning = false;
	std::atomic<UINT64> nextTimerId = 1;
};