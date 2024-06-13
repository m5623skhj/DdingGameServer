#pragma once
#include <MSWSock.h>
#include <vector>
#include <thread>
#include <unordered_map>
#include <optional>
#include "DefineType.h"
#include "EnumType.h"
#include "NetServerSerializeBuffer.h"
#include "RIOSession.h"

#pragma comment(lib, "ws2_32.lib")

const DWORD RIO_BUFFER_MEMORY_POOL_CHUNK_SIZE = 10;
const DWORD SEND_BUFFER_SIZE = 2048;
const DWORD RIO_PENDING_SEND = 8192;
constexpr DWORD TOTAL_BUFFER_SIZE = SEND_BUFFER_SIZE * RIO_PENDING_SEND;

struct TickSet
{
	UINT64 nowTick = 0;
	UINT64 beforeTick = 0;
};

struct IOContextResult
{
	IOContext* ioContext;
	std::shared_ptr<RIOSession> session;
};

class RIOSession;

class RIOServer
{
protected:
	RIOServer();
	virtual ~RIOServer() = default;

public:
	static RIOServer& GetInst()
	{
		static RIOServer instance;
		return instance;
	}

public:
	bool StartServer(const std::wstring& optionFileName);
	void StopServer();

#pragma region thread
public:
	void Accepter();
	void Worker(BYTE inThreadId);
	
private:
	void RunThreads();
	FORCEINLINE void SleepRemainingFrameTime(OUT TickSet& tickSet);

	ULONG RIODequeueCompletion(RIO_CQ& rioCQ, RIORESULT* rioResults);
	std::optional<IOContextResult> GetIOCompletedContext(RIORESULT& rioResult);

	IO_POST_ERROR IOCompleted(IOContext& context, ULONG transferred, RIOSession& session, BYTE threadId);

	IO_POST_ERROR RecvIOCompleted(ULONG transferred, RIOSession& session, BYTE threadId);
	IO_POST_ERROR SendIOCompleted(ULONG transferred, RIOSession& session, BYTE threadId);
	void InvalidIOCompleted(IOContext& context);
private:
	IO_POST_ERROR RecvCompleted(RIOSession& session, DWORD transferred);
	IO_POST_ERROR SendCompleted(RIOSession& session);

	FORCEINLINE WORD GetPayloadLength(OUT NetBuffer& buffer, int restSize);

private:
	BYTE GetMinimumSessionThreadId() const;

private:
	float GetSessionRationInThisThread(BYTE threadId);
	bool CheckRebalancingSession(BYTE threadId);
	void RebalanceSessionToThread();

private:
	std::thread accepterThread;
	std::vector<std::thread> workerThreads;
	
	const float sessionRatioInThread = 50.0f;
	bool serverStop = false;
#pragma endregion thread

#pragma region rio
private:
	bool InitializeRIO();

private:
	GUID functionTableId = WSAID_MULTIPLE_RIO;

	RIO_CQ* rioCQList;
	
	std::shared_ptr<char> rioSendBuffer = nullptr;

	RIO_NOTIFICATION_COMPLETION rioNotiCompletion;
	RIO_EXTENSION_FUNCTION_TABLE rioFunctionTable;
	OVERLAPPED rioCQOverlapped;

	bool* workerOnList;

public:
	RecvRIOBuffer* GetRecvRIOBuffer();
	SendRIOBuffer* GetSendRIOBuffer();
	void ReleaseRIORecvBuffer(RecvRIOBuffer* releaseBuffer);
	void ReleaseRIOSendBuffer(SendRIOBuffer* releaseBuffer);

private:
	void ReserveRIOBuffer();

private:
	CTLSMemoryPool<RecvRIOBuffer>* recvRIOBufferPool = nullptr;
	CTLSMemoryPool<SendRIOBuffer>* sendRIOBufferPool = nullptr;
#pragma endregion rio

#pragma region io
public:
	void SendPacket(OUT RIOSession& session, OUT NetBuffer& packet);

private:
	IO_POST_ERROR RecvPost(OUT RIOSession& session);
	IO_POST_ERROR SendPost(OUT RIOSession& session);

	ULONG MakeSendStream(OUT RIOSession& session);
	bool IsValidPacketSize(int bufferSize);
private:
	CTLSMemoryPool<IOContext> contextPool;
#pragma endregion io

#pragma region serverOption
private:
	bool ServerOptionParsing(const std::wstring& optionFileName);
	bool SetSocketOption();

private:
	BYTE numOfWorkerThread = 0;
	bool nagleOn = false;
	WORD port = 0;
	UINT maxClientCount = 0;
#pragma endregion serverOption

private:
	SOCKET listenSocket;

#pragma region session
public:
	UINT GetSessionCount() const;
	std::shared_ptr<RIOSession> GetSession(SessionId sessionId);

	void Disconnect(SessionId sessionId);

private:
	virtual std::shared_ptr<RIOSession> GetNewSession(SOCKET enteredClientSocket, BYTE threadId);
	bool MakeNewSession(SOCKET enteredClientSocket, const std::wstring_view& enteredClientIP);
	FORCEINLINE bool ReleaseSession(OUT RIOSession& releaseSession);

	FORCEINLINE void IOCountDecrement(RIOSession& session);

protected:
	SessionId nextSessionId = INVALID_SESSION_ID + 1;

private:
	std::unordered_map<SessionId, std::shared_ptr<RIOSession>> sessionMap;
	SRWLOCK sessionMapLock;

	UINT sessionCount = 0;

private:
	std::vector<UINT> numOfSessionInWorkerThread;
#pragma endregion session
};