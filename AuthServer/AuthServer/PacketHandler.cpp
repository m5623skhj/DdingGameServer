#include "PreCompile.h"
#include "AuthServer.h"
#include "AuthProtocol.h"

void AuthServer::HandleC2AuthPacket(UINT64 sessionId, WORD packetType, CNetServerSerializationBuf& recvBuffer)
{
	switch (packetType)
	{
		// 필요한 패킷
		/*
			1. 로그인 패킷(ID랑 패스워드)
				AuthDB에 위 정보로 PC? User?를 SELECT
				SELECT 결과 받으면
					실패시 클라이언트에게 실패 전송
					성공시 게임 서버에게(PCDBID? UserDBID?, 토큰?을 송신
					해당 패킷 송신이 성공하면 요청한 클라이언트에게 로그인 성공 패킷 송신
					이 때, GameServer의 IP랑 Port도 공유하는게 좋을런지?
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