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
	virtual ~PC() override = default;
	explicit PC(RIOSession& inSession, PCDBID inPCId);

public:
	void SendPacket(IGameAndClientPacket& packet);
	void SendPacket(NetBuffer& packet);
	void SendPacketAndDisconnect(IGameAndClientPacket& packet);
	void SendPacketAndDisconnect(NetBuffer& packet);

	void Disconnect();

public:
	SessionId GetSessionId() const { return session.GetSessionId(); }
	PCDBID GetPCId() const { return pcDBID; }

public:
	virtual void OnEnterWorld() override;
	virtual void OnLeaveWorld() override;
	virtual void OnDamaged() override;
	virtual void OnDied() override;
	virtual void OnRevived() override;

public:
	bool LoadFromDB();
	bool OnLoadFromDB();
	bool FinalizeLoadFromDB();
	bool OnFinalizeLoadFromDB();
	bool OnDBLoadCompleted();

public:
	void UpdateByPing();

public:
	void DBJobLazyRunnerPush(DBJobTimeLazyRunner&& lazyRunnerInst);

private:
	void FireLazyRunnerIfConditionMet(const UINT64 now);

private:
	std::priority_queue<DBJobTimeLazyRunner, std::vector<DBJobTimeLazyRunner>, std::less<DBJobTimeLazyRunner>> timeLazyRunnerHolder;

private:
	// Enter the number of SPs that need to be loaded from the DB
	// If there is nothing to load, enter 0
	static const int16_t totalDBLoadCount{0};
	// Enter the number of SPs that need to be loaded from the FinalizeLoad DB
	// If there is nothing to load, enter 0
	static const int16_t totalDBFinalizeLoadCount{0};

	std::atomic_int16_t dbLoadCount{};

private:
	PCDBID pcDBID;

private:
	RIOSession& session;
};