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
	// OAuth�� ����ؾ��ϳ�?
	// id�� ��� ���µ�, �н������ ��ȣȭ ��Ű�� �ؾ��ϴµ� ��� �����ؾ�����?
	/*
		1. �α��� ��Ŷ(ID�� �н�����)
			AuthDB�� �� ������ PC? User?�� SELECT
			SELECT ��� ������
				���н� Ŭ���̾�Ʈ���� ���� ����
				������ ���� ��������(PCDBID? UserDBID?, ��ū?�� �۽�
				�ش� ��Ŷ �۽��� �����ϸ� ��û�� Ŭ���̾�Ʈ���� �α��� ���� ��Ŷ �۽�
				�� ��, GameServer�� IP�� Port�� �����ϴ°� ��������?
	*/

	// DB�� ���� �Դ� ���� �ؾ��ϹǷ�, ��Ŷ�� ���⿡�� ó�������� ����
	// DB ó���� ���Ŀ� ��Ʈ���ؾ��� ��?
	// Auth DB�� AuthServer�� ���� ����Ǿ� �����Ƿ�, DB ó�� �����带 �ϳ� ���� ������ �� �� ����
	// �����忡 ����Ʈ ��ų ��, ���� ť�� �ϰ� ���� ������ �� ��
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