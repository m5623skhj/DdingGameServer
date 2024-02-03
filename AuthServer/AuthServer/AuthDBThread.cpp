#include "PreCompile.h"
#include "AuthDBThread.h"

bool AuthDBThreadManager::StartThreads(BYTE numOfThreads)
{
	if (isStarted == true)
	{
		std::cout << "Auth db threads already started" << std::endl;
		return false;
	}

	iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, numOfThreads);
	if (iocpHandle == NULL)
	{
		std::cout << "Auth db threads manager iocp handle is NULL" << std::endl;
		return false;
	}

	for (BYTE i = 0; i < numOfThreads; ++i)
	{
		workerThreads.emplace_back([this]() { this->Worker(); });
	}

	isStarted = true;
	return true;
}

void AuthDBThreadManager::StopThreads()
{

}

void AuthDBThreadManager::Worker()
{
	DWORD transferred;
	LPOVERLAPPED overlapped;
	DBJobObject* dbJobObject;

	while (true)
	{
		transferred = 0;
		dbJobObject = nullptr;
		overlapped = NULL;

		if (GetQueuedCompletionStatus(iocpHandle, &transferred, (PULONG_PTR)&dbJobObject, &overlapped, INFINITE) == TRUE)
		{

		}
		else
		{

		}
	}
}