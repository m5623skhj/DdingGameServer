#pragma once

#include <MSWSock.h>
#include "Ringbuffer.h"
#include "LockFreeQueue.h"
#include "Queue.h"
#include "EnumType.h"
#include "DefineType.h"
#include "NetServerSerializeBuffer.h"
#include <mutex>

class RIOServer;
class RIOSession;
class IGameAndClientPacket;

struct IOContext : RIO_BUF
{
	IOContext() = default;
	~IOContext() = default;

	void InitContext(SessionId inOwnerSessionId, RIO_OPERATION_TYPE inIOType);

	SessionId ownerSessionId = 0;
	RIO_OPERATION_TYPE ioType = RIO_OPERATION_TYPE::OP_ERROR;
};

struct RecvItem
{
	WORD bufferCount = 0;
	CRingbuffer recvRingBuffer;
	RIO_BUFFERID recvBufferId;
};

struct SendItem
{
	WORD bufferCount = 0;
	CLockFreeQueue<NetBuffer*> sendQueue;
	char rioSendBuffer[MAX_SEND_BUFFER_SIZE];
	NetBuffer* reservedBuffer = nullptr;
	RIO_BUFFERID sendBufferId;
	IO_MODE ioMode = IO_MODE::IO_NONE_SENDING;
};

struct RecvRIOBuffer : CRingbuffer
{
public :
	virtual ~RecvRIOBuffer()
	{
		InitPointer();
	}

public:
	RIO_BUFFERID recvBufferId;
};

struct SendRIOBuffer
{
	char rioSendBuffer[MAX_SEND_BUFFER_SIZE];
	RIO_BUFFERID sendBufferId;
};

class RIOSession
{
	friend RIOServer;

public:
	RIOSession() = delete;
	explicit RIOSession(SOCKET socket, SessionId sessionId, BYTE threadId);
	virtual ~RIOSession() = default;

private:
	bool InitSession(const RIO_EXTENSION_FUNCTION_TABLE& rioFunctionTable, RIO_NOTIFICATION_COMPLETION& rioNotiCompletion, RIO_CQ& rioRecvCQ, RIO_CQ& rioSendCQ);

public:
	virtual void OnClientEntered(const std::wstring_view& enteredClientIP);
	virtual void OnClientLeaved() {}

public:
	// NetBuffer로 SendPacket을 시도할 때는, NetBuffer::Alloc()로 만들어진 패킷만 가능합니다.
	// 위의 방법이 아닌, 임의로 만들어진 패킷에 대해서는 정의되지 않은 동작을 일으킬 수 있습니다.
	void SendPacket(NetBuffer& packet);
	void SendPacket(IGameAndClientPacket& packet);
	// NetBuffer로 SendPacketAndDisconnect을 시도할 때는, NetBuffer::Alloc()로 만들어진 패킷만 가능합니다.
	// 위의 방법이 아닌, 임의로 만들어진 패킷에 대해서는 정의되지 않은 동작을 일으킬 수 있습니다.
	void SendPacketAndDisconnect(NetBuffer& packet);
	void SendPacketAndDisconnect(IGameAndClientPacket& packet);
	void Disconnect();

private:
	void OnRecvPacket(NetBuffer& packet);

private:
	void OnSessionReleased(const RIO_EXTENSION_FUNCTION_TABLE& rioFunctionTable);

public:
	bool IsReleasedSession() { return isReleasedSession; }
	SessionId GetSessionId() const { return sessionId; }

private:
	SOCKET socket;
	SessionId sessionId = INVALID_SESSION_ID;
	
	bool sendDisconnect = false;
	bool ioCancle = false;

	std::atomic_bool isReleasedSession = false;

	BYTE threadId = UCHAR_MAX;

#pragma region IO
private:
	LONG ioCount = 0;

	RecvItem recvItem;
	SendItem sendItem;

	RIO_RQ rioRQ = RIO_INVALID_RQ;

	ULONG rioRecvOffset = 0;
#pragma endregion IO
};