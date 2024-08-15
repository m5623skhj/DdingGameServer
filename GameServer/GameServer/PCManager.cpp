#include "PreCompile.h"
#include "PCManager.h"
#include "PC.h"
#include "RIOSession.h"

PCManager& PCManager::GetInst()
{
	static PCManager instance;
	return instance;
}

void PCManager::InsertPC(RIOSession& session, PCDBID id)
{
	auto pc = std::make_shared<PC>(session, id);
	if (pc == nullptr)
	{
		g_Dump.Crash();
	}

	{
		std::lock_guard<std::shared_mutex> lock(sessionIdToPCMapLock);
		const auto& insertResult = sessionIdToPCMap.insert({ session.GetSessionId(), pc});
		if (insertResult.second == false)
		{
			// 세션 Id가 겹칠 일은 없겠지만, 디버깅 용으로 Crash를 둠
			g_Dump.Crash();
		}
	}
	{
		std::lock_guard<std::shared_mutex> lock(pcIdToPCMapLock);
		const auto& insertResult = pcIdToPCMap.insert({ pc->GetPCId(), pc});
		if (insertResult.second == false)
		{
			auto duplicatedPCIter = pcIdToPCMap.find(pc->GetPCId());
			if (duplicatedPCIter == pcIdToPCMap.end())
			{
				// 이럴 경우는 없겠지만, 디버깅 용으로 Crash를 둠
				g_Dump.Crash();
			}
			duplicatedPCIter->second->Disconnect();
			session.Disconnect();
			return;
		}
	}
}

void PCManager::ErasePCBySessionId(SessionId sessionId)
{
	PCDBID pcId = INVALID_PC_ID;
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

void PCManager::ErasePCByPCId(PCDBID pcId)
{
	SessionId sessionId = INVALID_SESSION_ID;
	{
		std::lock_guard<std::shared_mutex> lock(pcIdToPCMapLock);
		auto iter = pcIdToPCMap.find(pcId);
		if (iter == pcIdToPCMap.end())
		{
			return;
		}

		sessionId = iter->second->GetSessionId();
		pcIdToPCMap.erase(pcId);
	}

	{
		std::lock_guard<std::shared_mutex> lock(sessionIdToPCMapLock);
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
	std::shared_lock<std::shared_mutex> lock(sessionIdToPCMapLock);
	auto findIter = sessionIdToPCMap.find(sessionId);
	if (findIter == sessionIdToPCMap.end())
	{
		return nullptr;
	}

	return findIter->second;
}

std::shared_ptr<PC> PCManager::FindPCByPCId(PCDBID pcId)
{
	std::shared_lock<std::shared_mutex> lock(pcIdToPCMapLock);
	auto findIter = pcIdToPCMap.find(pcId);
	if (findIter == pcIdToPCMap.end())
	{
		return nullptr;
	}

	return findIter->second;
}