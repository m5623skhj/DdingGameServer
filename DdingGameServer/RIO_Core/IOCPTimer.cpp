#include "PreCompile.h"
#include "IOCPTimer.h"

IOCPTimerManager::IOCPTimerManager()
{
	timerThread = std::thread([this]() { this->RunTimerThread(); });
}

IOCPTimerManager::~IOCPTimerManager()
{
	timerThread.join();
}

void IOCPTimerManager::RunTimerThread()
{
	while (true)
	{

	}
}