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