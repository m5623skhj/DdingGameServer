#pragma once
#include "RIOServer.h"

class GameServerImpl final : public RIOServer
{
private:
	GameServerImpl() = default;
	virtual ~GameServerImpl() = default;
	GameServerImpl(const GameServerImpl& other) = delete;
	GameServerImpl& operator=(const GameServerImpl& other) = delete;

public:
	static GameServerImpl& GetInst()
	{
		static GameServerImpl instance;
		return instance;
	}

private:
	virtual std::shared_ptr<RIOSession> GetNewSession(SOCKET enteredClientSocket, BYTE threadId) override;
};