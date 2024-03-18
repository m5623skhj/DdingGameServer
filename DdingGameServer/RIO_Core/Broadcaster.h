#pragma once
#include <set>
#include <mutex>
#include "DefineType.h"
#include "Protocol.h"
#include "NetServerSerializeBuffer.h"

class RIOServer;

class Broadcaster
{
	friend RIOServer;

private:
	Broadcaster() = default;
	~Broadcaster() = default;

	Broadcaster(const Broadcaster&) = delete;
	Broadcaster& operator=(const Broadcaster&) = delete;

public:
	static Broadcaster& GetInst();

public:
	void BraodcastToAllSession(IGameAndClientPacket&& packet);
	//void BraodcastToAllSession(NetBuffer& packet);

private:
	void OnSessionEntered(SessionId enteredSessionId);
	void OnSessionLeaved(SessionId enteredSessionId);

private:
	std::mutex sessionSetLock;
	std::set<SessionId> sessionIdSet;
};