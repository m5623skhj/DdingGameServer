#include "PreCompile.h"
#include "IOCPTimer.h"

IOCPTimer::IOCPTimer()
	: wakeUpTime(0)
{
	timerId = IOCPTimerManager::GetInst().GetTimerId();
}

void IOCPTimer::ResumeTimer()
{
	IOCPTimerManager::GetInst().ResumeTimer(timerId, &overlapped);
}

IOCPTimerManager::IOCPTimerManager()
{
	iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 1);
	if (iocpHandle == NULL)
	{
		isValid = false;
		std::cout << "IOCP handle is NULL" << std::endl;
	}

	timerThread = std::thread([this]() { this->RunTimerThread(); });
}

IOCPTimerManager::~IOCPTimerManager()
{
	if (isRunning == true)
	{
		StopTimerManager();
	}

	timerThread.join();
}

void IOCPTimerManager::RunTimerThread()
{
	if (isValid == false)
	{
		std::cout << "IOCPTimerManager state is invalid" << std::endl << "Unuse IOCP Timer" << std::endl;
		return;
	}
	isRunning = true;

	bool successGQCS;
	UINT64 timerId;
	LPOVERLAPPED lpoverlapped;
	DWORD unusedTransferred;

	while (true)
	{
		successGQCS = GetQueuedCompletionStatus(iocpHandle, &unusedTransferred, (PULONG_PTR)&timerId, &lpoverlapped, INFINITE);
		if (lpoverlapped == NULL)
		{
			std::cout << "IOCPTimerManager : overlapped is NULL" << std::endl;
			g_Dump.Crash();
		}
		else if (lpoverlapped == (LPOVERLAPPED)1)
		{
			if (timerId != 0)
			{
				std::cout << "IOCPTimerManager : Invalid lpoverlapped with timerId " << timerId << std::endl;
				g_Dump.Crash();
			}

			break;
		}
		
		std::shared_ptr<IOCPTimer> timer = nullptr;
		{
			std::lock_guard<std::mutex> lock(timerMapLock);
			auto iter = timerMap.find(timerId);
			if (iter == timerMap.end())
			{
				std::cout << "IOCPTimerManager : Invalid timer map id " << timerId << std::endl;
				continue;
			}

			timer = iter->second;
		}

		timer->OnTimer();
	}

	isRunning = false;
}

void IOCPTimerManager::RegisterTimer(std::shared_ptr<IOCPTimer> timer)
{
	std::lock_guard<std::mutex> lock(timerMapLock);
	timerMap.insert({ timer->GetTimerId(), timer });
	std::cout << "IOCPTimerManager : Timer registered id " << timer->GetTimerId() << std::endl;
}

void IOCPTimerManager::ResumeTimer(UINT64 timerId, LPOVERLAPPED timerOverlapped)
{
	PostQueuedCompletionStatus(iocpHandle, 0, timerId, timerOverlapped);
}

void IOCPTimerManager::StopTimer(UINT64 timerId)
{
	std::lock_guard<std::mutex> lock(timerMapLock);
	timerMap.erase(timerId);
	std::cout << "IOCPTimerManager : Timer stopped id " << timerId << std::endl;
}

void IOCPTimerManager::StopTimerManager()
{
	PostQueuedCompletionStatus(iocpHandle, 0, 0, (LPOVERLAPPED)1);
}