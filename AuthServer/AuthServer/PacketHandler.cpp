#include "PreCompile.h"
#include "AuthServer.h"
#include "AuthProtocol.h"
#include "LanServerSerializeBuf.h"
#include "NetServerSerializeBuffer.h"
#include "AuthQuery.h"

void AuthServer::HandleC2AuthPacket(UINT64 sessionId, WORD packetType, CNetServerSerializationBuf& recvBuffer)
{
	switch (packetType)
	{
	case AuthProtocol::C2Auth_LoginRequest:
	{
		std::string id, password;
		recvBuffer.ReadBuffer(id);
		recvBuffer.ReadBuffer(password);

		ClientLogin(sessionId, id, password);
		break;
	}
	default:
		break;
	}
}

void AuthServer::ClientLogin(UINT64 sessionId, std::string_view id, std::string_view password)
{
	// OAuth를 사용해야하나?
	// id는 상관 없는데, 패스워드는 암호화 시키긴 해야하는데 어떻게 관리해야할지?
	/*
		1. 로그인 패킷(ID랑 패스워드)
			AuthDB에 위 정보로 PC? User?를 SELECT
			SELECT 결과 받으면
				실패시 클라이언트에게 실패 전송
				성공시 게임 서버에게(PCDBID? UserDBID?, 토큰?을 송신
				해당 패킷 송신이 성공하면 요청한 클라이언트에게 로그인 성공 패킷 송신
				이 때, GameServer의 IP랑 Port도 공유하는게 좋을런지?
	*/

	// DB에 직접 왔다 갔다 해야하므로, 패킷을 여기에서 처리하지는 않음
	// DB 처리한 이후에 컨트롤해야할 듯?
	// Auth DB와 AuthServer가 직접 연결되어 있으므로, DB 처리 스레드를 하나 따로 만들어야 할 것 같음
	// 스레드에 포스트 시킬 때, 공용 큐로 일갑 집어 넣으면 될 듯
}

void AuthServer::HandleGame2AuthPacket(UINT64 sessionId, WORD packetType, CSerializationBuf& recvBuffer)
{
	switch (packetType)
	{
	case AuthProtocol::Game2Auth_Register:
	{
		GameServerId gameServerId = 0;
		recvBuffer >> gameServerId;

		GameServerRegisterRequest(sessionId, gameServerId);
	}
		break;
	default:
		break;
	}
}

#pragma region AuthProtocolHandler
void AuthServer::GameServerRegisterRequest(UINT64 sessionId, GameServerId gameServerId)
{
	CSerializationBuf* packet = CSerializationBuf::Alloc();
	WORD packetType = AuthProtocol::Auth2Game_RegisterSuccess;
	*packet << packetType;

	bool isConnected = true;
	{
		std::unique_lock<std::shared_mutex> lock(gameServerMapLock);
		for (const auto& item : gameServerMap)
		{
			if (item.second == gameServerId)
			{
				std::cout << "Duplicated game server id : " << gameServerId << std::endl;
				isConnected = false;
				break;
			}
		}

		if (isConnected == true)
		{
			gameServerMap.insert({ sessionId, gameServerId });
		}
	}
	*packet << isConnected;
	SendPacketToLanClient(sessionId, packet);
}

#pragma endregion AuthProtocolHandler