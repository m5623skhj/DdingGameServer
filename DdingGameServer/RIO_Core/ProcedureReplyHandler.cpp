#include "PreCompile.h"
#include "ProcedureReplyHandler.h"
#include "LanServerSerializeBuf.h"
#include "NetServerSerializeBuffer.h"
#include "PacketManager.h"
#include "RIOServer.h"
#include "DBJob.h"

void ProcedureReplyHandler::Initialize()
{
	REGISTER_ALL_DB_REPLY_HANDLER();
}

void ProcedureReplyHandler::SPReplyHandle(UINT packetId, OUT CSerializationBuf& recvPacket)
{
	auto packetHandler = ProcedureReplyHandler::GetInst().GetPacketHandler(packetId);
	if(packetHandler == nullptr)
	{
		return;
	}

	auto packet = PacketManager::GetInst().MakePacket(packetId);
	if (packet == nullptr)
	{
		return;
	}

	char* targetPtr = reinterpret_cast<char*>(packet.get()) + sizeof(char*);
	memcpy(targetPtr, recvPacket.GetReadBufferPtr(), recvPacket.GetUseSize());
	std::any anyPacket = std::any(packet.get());
	packetHandler(anyPacket, recvPacket);
}

DBPacketReplyHandler ProcedureReplyHandler::GetPacketHandler(UINT packetId)
{
	auto iter = packetHandlerMap.find(packetId);
	if (iter == packetHandlerMap.end())
	{
		return nullptr;
	}

	return iter->second;
}

bool ProcedureReplyHandler::AssemblePacket(DBJobReply& packet, OUT CSerializationBuf& recvPacket)
{
	auto batchJob = DBJobManager::GetInst().GetRegistedDBJob(packet.jobKey);
	if (batchJob == nullptr)
	{
		std::cout << "AssemblePacket() : Invalid jobkey" << std::endl;
		return true;
	}

	if (packet.isSuccessed == false)
	{
		std::cout << "AssemblePacket() : DB job was failed" << std::endl;
		batchJob->OnRollback();

		return true;
	}

	batchJob->OnCommit();
	return true;
}
