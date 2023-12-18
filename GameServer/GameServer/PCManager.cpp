#include "PreCompile.h"
#include "PCManager.h"

PCManager& PCManager::GetInst()
{
	static PCManager instance;
	return instance;
}

void PCManager::AddPCBySessionId(SessionId sessionId, std::shared_ptr<PC> pc)
{

}

void PCManager::AddPCByPCId(PCId pcId, std::shared_ptr<PC> pc)
{

}

void PCManager::DeletePCBySessionId(SessionId sessionId)
{

}

void PCManager::DeletePCByPCId(PCId pcId)
{

}

std::shared_ptr<PC> PCManager::FindPCBySessionId(SessionId sessionId)
{
	std::lock_guard<std::mutex> lock(sessionIdToPCMapLock);
	auto findIter = sessionIdToPCMap.find(sessionId);
	if (findIter == sessionIdToPCMap.end())
	{
		return nullptr;
	}

	return findIter->second;
}

std::shared_ptr<PC> PCManager::FindPCByPCId(PCId pcId)
{
	std::lock_guard<std::mutex> lock(pcIdToPCMapLock);
	auto findIter = pcIdToPCMap.find(pcId);
	if (findIter == pcIdToPCMap.end())
	{
		return nullptr;
	}

	return findIter->second;
}