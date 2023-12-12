#include "PreCompile.h"
#include "RIOSession.h"
#include "PacketManager.h"
#include "RIOServer.h"

#include "BuildConfig.h"
#include "DefineType.h"
#include "EnumType.h"

void IOContext::InitContext(SessionId inOwnerSessionId, RIO_OPERATION_TYPE inIOType)
{
	ownerSessionId = inOwnerSessionId;
	ioType = inIOType;
}

RIOSession::RIOSession(SOCKET inSocket, SessionId inSessionId, BYTE inThreadId)
	: socket(inSocket)
	, sessionId(inSessionId)
	, threadId(inThreadId)
{
	sendItem.reservedBuffer = nullptr;
}

bool RIOSession::InitSession(const RIO_EXTENSION_FUNCTION_TABLE& rioFunctionTable, RIO_NOTIFICATION_COMPLETION& rioNotiCompletion, RIO_CQ& rioRecvCQ, RIO_CQ& rioSendCQ)
{
	u_long arg = 1;
	ioctlsocket(socket, FIONBIO, &arg);

	recvItem.recvRingBuffer.InitPointer();

	recvItem.recvBufferId = rioFunctionTable.RIORegisterBuffer(recvItem.recvRingBuffer.GetBufferPtr(), DEFAULT_RINGBUFFER_MAX);
	if (recvItem.recvBufferId == RIO_INVALID_BUFFERID)
	{
		return false;
	}

	sendItem.sendBufferId = rioFunctionTable.RIORegisterBuffer(sendItem.rioSendBuffer, MAX_SEND_BUFFER_SIZE);
	if (sendItem.sendBufferId == RIO_INVALID_BUFFERID)
	{
		return false;
	}

	rioRQ = rioFunctionTable.RIOCreateRequestQueue(socket, 32, 1, 32, 1, rioRecvCQ, rioSendCQ, &sessionId);
	if (rioRQ == RIO_INVALID_RQ)
	{
		return false;
	}

	return true;
}

void RIOSession::SendPacket(IGameAndClientPacket& packet)
{
	NetBuffer* buffer = NetBuffer::Alloc();
	if (buffer == nullptr)
	{
		std::cout << "buffer is nullptr" << std::endl;
		return;
	}

	*buffer << packet.GetPacketId();
	packet.PacketToBuffer(*buffer);

	SendPacket(*buffer);
}

void RIOSession::SendPacket(NetBuffer& packet)
{
	RIOServer::GetInst().SendPacket(*this, packet);
}

void RIOSession::SendPacketAndDisconnect(IGameAndClientPacket& packet)
{
	sendDisconnect = true;
	NetBuffer* buffer = NetBuffer::Alloc();
	if (buffer == nullptr)
	{
		std::cout << "buffer is nullptr" << std::endl;
		return;
	}

	*buffer << packet.GetPacketId();
	packet.PacketToBuffer(*buffer);

	SendPacket(*buffer);
}

void RIOSession::SendPacketAndDisconnect(NetBuffer& packet)
{
	sendDisconnect = true;
	RIOServer::GetInst().SendPacket(*this, packet);
}

void RIOSession::Disconnect()
{
	RIOServer::GetInst().Disconnect(sessionId);
}

void RIOSession::OnRecvPacket(NetBuffer& recvPacket)
{
	PacketId packetId;
	recvPacket >> packetId;

	auto packetHandler = PacketManager::GetInst().GetPacketHandler(packetId);
	if (packetHandler == nullptr)
	{
		return;
	}
	
	auto packet = PacketManager::GetInst().MakePacket(packetId);
	if (packet == nullptr)
	{
		return;
	}

	char* targetPtr = reinterpret_cast<char*>(packet.get()) + sizeof(char*);
	std::any anyPacket = std::any(packet.get());
	packetHandler(*this, recvPacket, anyPacket);
}

void RIOSession::OnSessionReleased(const RIO_EXTENSION_FUNCTION_TABLE& rioFunctionTable)
{
	rioFunctionTable.RIODeregisterBuffer(recvItem.recvBufferId);
	rioFunctionTable.RIODeregisterBuffer(sendItem.sendBufferId);
}