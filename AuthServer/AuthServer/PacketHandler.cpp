#include "PreCompile.h"
#include "AuthServer.h"
#include "AuthProtocol.h"

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

}