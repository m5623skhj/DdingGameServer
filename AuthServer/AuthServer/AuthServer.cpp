#include "PreCompile.h"
#include "AuthServer.h"

bool AuthLanServer::Start(const std::wstring& optionFile)
{
	if (CLanServer::Start(optionFile.c_str()) == false)
	{
		return false;
	}

	return true;
}

void AuthLanServer::OnClientJoin(UINT64 OutClientID)
{

}

void AuthLanServer::OnClientLeave(UINT64 ClientID)
{

}

bool AuthLanServer::OnConnectionRequest()
{
	return true;
}

void AuthLanServer::OnRecv(UINT64 ReceivedSessionID, CSerializationBuf* OutReadBuf)
{

}

void AuthLanServer::OnSend(UINT64 ClientID, int sendsize)
{

}

void AuthLanServer::OnWorkerThreadBegin()
{

}

void AuthLanServer::OnWorkerThreadEnd()
{

}

void AuthLanServer::OnError(st_Error* OutError)
{

}

bool AuthNetServer::Start(const std::wstring& optionFile)
{
	if (CNetServer::Start(optionFile.c_str()) == false)
	{
		return false;
	}

	return true;
}


void AuthNetServer::OnClientJoin(UINT64 OutClientID)
{

}

void AuthNetServer::OnClientLeave(UINT64 ClientID)
{

}

bool AuthNetServer::OnConnectionRequest(const WCHAR* IP)
{
	return true;
}

void AuthNetServer::OnRecv(UINT64 ReceivedSessionID, CNetServerSerializationBuf* OutReadBuf)
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
}

void AuthNetServer::OnSend(UINT64 ClientID, int sendsize)
{

}

void AuthNetServer::OnWorkerThreadBegin()
{

}

void AuthNetServer::OnWorkerThreadEnd()
{

}

void AuthNetServer::OnError(st_Error* OutError)
{

}

void AuthNetServer::GQCSFailed(int LastError, UINT64 ClientID)
{

}

bool AuthServer::StartAuthServer(const std::wstring& lanServerOptionFile, const std::wstring& netServerOptionFile)
{
	if (authLanServer.Start(lanServerOptionFile) == false ||
		authNetServer.Start(netServerOptionFile) == false)
	{
		return false;
	}

	return true;
}