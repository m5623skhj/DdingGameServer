#pragma once
#include "Creature.h"
#include "GameServerDefine.h"
#include <memory>
#include "DefineType.h"
#include "RIOSession.h"
#include "DBJobLazyRunner.h"
#include <queue>

class PC : public Creature
{
public:
	PC() = delete;
	virtual ~PC() = default;
	explicit PC(RIOSession& inSession, PCDBID inPCId);

public:
	void SendPacket(IGameAndClientPacket& packet);
	void SendPacket(NetBuffer& packet);
	void SendPacketAndDisconnect(IGameAndClientPacket& packet);
	void SendPacketAndDisconnect(NetBuffer& packet);

	void Disconnect();

public:
	SessionId GetSessionId() const { return pcDBID; }
	PCDBID GetPCId() const { return session.GetSessionId(); }

public:
	virtual void OnEnterWorld();
	virtual void OnLeaveWorld();
	virtual void OnDamaged();
	virtual void OnDied();
	virtual void OnRevived();

public:
	bool LoadFromDB();
	bool OnLoadFromDB();
	void FinalizeOnLoadFromDB();

public:
	void UpdateByPing();

public:
	void DBJobLazyRunnerPush(DBJobTimeLazyRunner&& lazyRunnerInst);

private:
	void FireLazyRunnerIfConditionMet(const UINT64 now);

private:
	std::priority_queue<DBJobTimeLazyRunner, std::vector<DBJobTimeLazyRunner>, std::less<DBJobTimeLazyRunner>> timeLazyRunnerHolder;

private:
	PCDBID pcDBID;

private:
	RIOSession& session;
};