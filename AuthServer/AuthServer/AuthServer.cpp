#include "PreCompile.h"
#include "AuthServer.h"
#include "LanServerSerializeBuf.h"
#include "NetServerSerializeBuffer.h"

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
	AuthServer::GetInst().OnGameServerDisconnected(ClientID);
}

bool AuthLanServer::OnConnectionRequest()
{
	return true;
}

void AuthLanServer::OnRecv(UINT64 sessionId, CSerializationBuf* receivedBuffer)
{
	WORD packetType;
	*receivedBuffer >> packetType;

	AuthServer::GetInst().HandleGame2AuthPacket(sessionId, packetType, *receivedBuffer);
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

void AuthNetServer::OnRecv(UINT64 sessionId, CNetServerSerializationBuf* receivedBuffer)
{
	WORD packetType;
	*receivedBuffer >> packetType;

	AuthServer::GetInst().HandleC2AuthPacket(sessionId, packetType, *receivedBuffer);
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

void AuthServer::OnGameServerConnected(UINT64 sessionId)
{

}

void AuthServer::OnGameServerDisconnected(UINT64 sessionId)
{
	{
		std::unique_lock<std::shared_mutex> lock(gameServerMapLock);
		auto iter = gameServerMap.find(sessionId);
		if (iter == gameServerMap.end())
		{
			return;
		}

		GameServerId disconnectedServerId = iter->second;
		gameServerMap.erase(sessionId);
		{
			std::unique_lock<std::shared_mutex> innerLock(userInfoMapLock);
			for(auto it = userInfoMap.begin(); it != userInfoMap.end();)
			{
				if (it->second.nowInServerId == disconnectedServerId)
				{
					it = userInfoMap.erase(it);
				}
				else
				{
					++it;
				}
			}
		}
	}
}

bool AuthServer::SendPacketToLanClient(UINT64 sessionId, CSerializationBuf* packet)
{
	return authLanServer.SendPacket(sessionId, packet);
}

bool AuthServer::SendPacketToNetClient(UINT64 sessionId, CNetServerSerializationBuf* packet)
{
	return authNetServer.SendPacket(sessionId, packet);
}