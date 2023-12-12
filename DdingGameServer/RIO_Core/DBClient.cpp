#include "PreCompile.h"
#include "DBClient.h"
#include "LanServerSerializeBuf.h"
#include "NetServerSerializeBuffer.h"
#include "ProcedureReplyHandler.h"
#include <memory>
#include "DBJob.h"
#include "Protocol.h"
#include "DBHelper.h"

DBClient::DBClient()
{
	ProcedureReplyHandler::GetInst().Initialize();
}

DBClient::~DBClient()
{
	Stop();
}

DBClient& DBClient::GetInstance()
{
	static DBClient instance;
	return instance;
}

void DBClient::Start(const std::wstring& optionFile)
{
	if (CMultiLanClient::Start(optionFile.c_str()) == false)
	{
		g_Dump.Crash();
	}
}

void DBClient::OnConnectionComplete()
{

}

void DBClient::OnRecv(CSerializationBuf* OutReadBuf)
{
	CSerializationBuf& buffer = *OutReadBuf;
	UINT recvPacketId = 0;
	buffer >> recvPacketId;

	ProcedureReplyHandler::GetInst().SPReplyHandle(recvPacketId, buffer);
}

void DBClient::OnSend()
{

}

void DBClient::OnWorkerThreadBegin()
{

}

void DBClient::OnWorkerThreadEnd()
{

}

void DBClient::OnError(st_Error* OutError)
{

}

void DBClient::OnDisconnect()
{

}

//void DBClient::CallProcedure(CSerializationBuf& packet)
//{
//	CMultiLanClient::SendPacket(&packet);
//}
//
//void DBClient::SendPacket(CSerializationBuf& packet)
//{
//	CMultiLanClient::SendPacket(&packet);
//}
//
void DBClient::SendPacketToFixedChannel(IGameAndDBPacket& packet, UINT64 sessionId)
{
	auto buffer = DBHelper::MakeProcedurePacket(packet.GetPacketId());
	packet.PacketToBuffer(buffer);

	CMultiLanClient::SendPacket(&buffer, sessionId);
}

void DBClient::SendPacketToFixedChannel(DBJob& dbJob)
{
	CMultiLanClient::SendPacket(dbJob.GetJobBuffer());
}