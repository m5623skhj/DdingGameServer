#include "PreCompile.h"
#include "PCManager.h"
#include "PC.h"
#include "../../DdingGameServer/RIO_Core/RIOSession.h"

PCManager& PCManager::GetInst()
{
	static PCManager instance;
	return instance;
}

void PCManager::AddPC(RIOSession& session)
{
	auto pc = std::make_shared<PC>(session, ++pcIdGenerator);
	if (pc == nullptr)
	{
		g_Dump.Crash();
	}

	{
		std::lock_guard lock(sessionIdToPCMapLock);
		sessionIdToPCMap.insert({ session.GetSessionId(), pc});
	}
	{
		std::lock_guard lock(pcIdToPCMapLock);
		pcIdToPCMap.insert({ pc->GetPCId(), pc});
	}
}

void PCManager::DeletePCBySessionId(SessionId sessionId)
{
	PCId pcId = INVALID_PC_ID;
	{
		std::lock_guard lock(sessionIdToPCMapLock);
		auto iter = sessionIdToPCMap.find(sessionId);
		if (iter == sessionIdToPCMap.end())
		{
			return;
		}

		pcId = iter->second->GetPCId();
		sessionIdToPCMap.erase(sessionId);
	}

	{
		std::lock_guard lock(pcIdToPCMapLock);
		auto iter = pcIdToPCMap.find(pcId);
		if (iter == pcIdToPCMap.end())
		{
			return;
		}

		pcIdToPCMap.erase(pcId);
	}
}

void PCManager::DeletePCByPCId(PCId pcId)
{
	SessionId sessionId = INVALID_SESSION_ID;
	{
		std::lock_guard lock(pcIdToPCMapLock);
		auto iter = pcIdToPCMap.find(pcId);
		if (iter == pcIdToPCMap.end())
		{
			return;
		}

		sessionId = iter->second->GetSessionId();
		pcIdToPCMap.erase(pcId);
	}

	{
		std::lock_guard lock(sessionIdToPCMapLock);
		auto iter = sessionIdToPCMap.find(sessionId);
		if (iter == sessionIdToPCMap.end())
		{
			return;
		}

		sessionIdToPCMap.erase(sessionId);
	}
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