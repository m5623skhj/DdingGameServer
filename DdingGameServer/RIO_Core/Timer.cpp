#include "PreCompile.h"
#include "Timer.h"
#include <list>

#define TIMER_THREAD_WAITING_TIME 100

Timer::Timer()
	: wakeUpTime(0)
{
	timerId = TimerManager::GetInst().GetTimerId();
}

void Timer::ResumeTimer()
{
	TimerManager::GetInst().ResumeTimer(timerId);
}

TimerManager::TimerManager()
{
	threadStopEventHandle = NULL;
	timerThread = std::thread([this]() { this->RunTimerThread(); });
}

TimerManager::~TimerManager()
{
	if (isRunning == true)
	{
		StopTimerManager();
	}

	timerThread.join();
}

void TimerManager::RunTimerThread()
{
	threadStopEventHandle = CreateEvent(NULL, FALSE, FALSE, NULL);

	if (threadStopEventHandle == NULL)
	{
		std::cout << "Timer Manager event handle is invalid" << std::endl;
		g_Dump.Crash();
	}

	isRunning = true;
	std::list<std::shared_ptr<Timer>> expiredTimerList;
	while (true)
	{
		auto result = WaitForSingleObject(threadStopEventHandle, TIMER_THREAD_WAITING_TIME);
		if (result == WAIT_OBJECT_0)
		{
			break;
		}

		GetExpiredTimer(expiredTimerList);
		for (auto& expiredTimer : expiredTimerList)
		{
			expiredTimer->OnTimer();
		}
		expiredTimerList.clear();
	}
	isRunning = false;
}

void TimerManager::GetExpiredTimer(std::list<std::shared_ptr<Timer>>& outList)
{
	const auto now = std::chrono::system_clock::now();
	auto nowTick = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
	{
		std::lock_guard lock(timerListLock);
		for (auto& iter : timerList)
		{
			if (nowTick > iter->GetNextWakeUpTime())
			{
				outList.push_back(iter);
				timerList.pop_front();
			}
			else
			{
				break;
			}
		}
	}
}

void TimerManager::RegisterTimer(std::shared_ptr<Timer> timer)
{
	std::lock_guard lock(registeredTimerMapLock);
	registeredTimerMap.insert({ timer->GetTimerId(), timer });
}

bool TimerManager::ResumeTimer(UINT64 timerId)
{
	std::shared_ptr<Timer> timer = nullptr;
	{
		std::lock_guard lock(registeredTimerMapLock);
		auto timerIter = registeredTimerMap.find(timerId);
		if (timerIter != registeredTimerMap.end())
		{
			return false;
		}

		timer = timerIter->second;
	}

	{
		std::lock_guard lock(timerListLock);
		auto end = timerList.end();
		for (auto iter = timerList.begin(); iter != end; ++iter)
		{
			if (timer->GetNextWakeUpTime() > timer->GetNextWakeUpTime())
			{
				timerList.insert(iter, timer);
				break;
			}
		}
	}

	return true;
}

void TimerManager::StopTimer(UINT64 timerId)
{
	std::lock_guard lock(timerListLock);
	auto end = timerList.end();
	for (auto iter = timerList.begin(); iter != end; ++iter)
	{
		if ((*iter)->GetTimerId() == timerId)
		{
			timerList.erase(iter);
			break;
		}
	}
}

void TimerManager::DeregisterTimer(UINT64 timerId)
{
	StopTimer(timerId);
	{
		std::lock_guard lock(registeredTimerMapLock);
		registeredTimerMap.erase(timerId);
	}
}

void TimerManager::StopTimerManager()
{
	SetEvent(threadStopEventHandle);
}