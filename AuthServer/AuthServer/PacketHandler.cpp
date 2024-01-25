#include "PreCompile.h"
#include "AuthServer.h"
#include "AuthProtocol.h"
#include "LanServerSerializeBuf.h"

void AuthServer::HandleC2AuthPacket(UINT64 sessionId, WORD packetType, CNetServerSerializationBuf& recvBuffer)
{
	switch (packetType)
	{
		// �ʿ��� ��Ŷ
		/*
			1. �α��� ��Ŷ(ID�� �н�����)
				AuthDB�� �� ������ PC? User?�� SELECT
				SELECT ��� ������
					���н� Ŭ���̾�Ʈ���� ���� ����
					������ ���� ��������(PCDBID? UserDBID?, ��ū?�� �۽�
					�ش� ��Ŷ �۽��� �����ϸ� ��û�� Ŭ���̾�Ʈ���� �α��� ���� ��Ŷ �۽�
					�� ��, GameServer�� IP�� Port�� �����ϴ°� ��������?
		*/
	case AuthProtocol::C2Auth_LoginRequest:

		break;
	default:
		break;
	}
}

void AuthServer::HandleGame2AuthPacket(UINT64 sessionId, WORD packetType, CSerializationBuf& recvBuffer)
{
	switch (packetType)
	{
	case AuthProtocol::Game2Auth_Register:
	{
		CSerializationBuf* packet = CSerializationBuf::Alloc();
		WORD pakcetType = AuthProtocol::Auth2Game_RegisterSuccess;
		*packet << packetType;
		bool isConnected = true;
		GameServerId gameServerId = 0;
		recvBuffer >> gameServerId;
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
		break;
	default:
		break;
	}
}