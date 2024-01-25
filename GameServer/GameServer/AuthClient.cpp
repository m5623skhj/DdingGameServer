#include "PreCompile.h"
#include "AuthClient.h"
#include "LanServerSerializeBuf.h"
#include <string>
#include "../../AuthServer/AuthServer/AuthProtocol.h"

AuthClient::AuthClient()
{

}

AuthClient::~AuthClient()
{
	Stop();
}

bool AuthClient::Start(const std::wstring& optionFile)
{
	if (CLanClient::Start(optionFile.c_str()) == false)
	{
		std::cout << "AuthClient::Start() failed" << std::endl;
		return false;
	}

	return true;
}

void AuthClient::OnConnectionComplete()
{
	CSerializationBuf* packet = CSerializationBuf::Alloc();
	WORD packetType = AuthProtocol::Game2Auth_Register;

	*packet << packetType/* << gameServerId*/;

	SendPacket(packet);
}

void AuthClient::OnRecv(CSerializationBuf* OutReadBuf)
{
	WORD packetType;
	*OutReadBuf >> packetType;

	switch (packetType)
	{
	case AuthProtocol::Game2Auth_Register:
		*OutReadBuf >> isConnected;
		if (isConnected == false)
		{
			std::cout << "Failed to connect to the auth server" << std::endl;
			g_Dump.Crash();
		}
		std::cout << "Successed to connect to auth server" << std::endl;
		break;
	default:
		break;
	}
}

void AuthClient::OnSend()
{

}

void AuthClient::OnWorkerThreadBegin()
{

}

void AuthClient::OnWorkerThreadEnd()
{

}

void AuthClient::OnError(st_Error* OutError)
{

}

void AuthClient::OnDisconnect()
{

}