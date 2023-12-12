#pragma once
#include <unordered_map>
#include "Ringbuffer.h"
//#include "Stack.h"
#include "LockFreeStack.h"
#include "LockFreeQueue.h"
#include "ServerCommon.h"
#include <list>

// 해당 소켓이 송신중에 있는지 아닌지
#define NONSENDING	0
#define SENDING		1
// Recv / Send Post 반환 값
#define POST_RETVAL_ERR_SESSION_DELETED		0
#define POST_RETVAL_ERR						1
#define POST_RETVAL_COMPLETE				2

#define IOCP_ONE_SEND_WSABUF_MAX			200

#define df_RELEASE_VALUE					0x100000000

struct st_Error;

class CSerializationBuf;

class CMultiLanClient
{
public:
	CMultiLanClient();
	virtual ~CMultiLanClient();

	bool Start(const WCHAR* szOptionFileName);
	void Stop();

public:
	bool SendPacket(CSerializationBuf* pSerializeBuf);
	bool SendPacket(CSerializationBuf* pSerializeBuf, UINT64 fixedId);

private:
	bool SendPacketImpl(CSerializationBuf* pSerializeBuf, UINT64 channelId);

public:
	// 서버에 Connect 가 완료 된 후
	virtual void OnConnectionComplete() = 0;

	// 패킷 수신 완료 후
	virtual void OnRecv(CSerializationBuf* OutReadBuf) = 0;
	// 패킷 송신 완료 후
	virtual void OnSend() = 0;

	// 워커스레드 GQCS 바로 하단에서 호출
	virtual void OnWorkerThreadBegin() = 0;
	// 워커스레드 1루프 종료 후
	virtual void OnWorkerThreadEnd() = 0;
	// 사용자 에러 처리 함수
	virtual void OnError(st_Error* OutError) = 0;
	// 이 세션이 서버에서 끊기면 호출
	virtual void OnDisconnect() {}

private:
	struct OVERLAPPEDIODATA
	{
		WORD		wBufferCount;
		OVERLAPPED  Overlapped;
		CRingbuffer RingBuffer;
	};

	struct OVERLAPPED_SEND_IO_DATA
	{
		LONG										lBufferCount;
		UINT										IOMode;
		OVERLAPPED									Overlapped;
		CLockFreeQueue<CSerializationBuf*>			SendQ;
	};

	struct Session
	{
		bool						bIsConnect = false;
		UINT						m_IOCount = 0;
		UINT64						m_SessionId = 0;
		OVERLAPPEDIODATA			m_RecvIOData;
		OVERLAPPED_SEND_IO_DATA		m_SendIOData;
		SOCKET						m_sock;
		CSerializationBuf* pSeirializeBufStore[IOCP_ONE_SEND_WSABUF_MAX];
	};

private:
	void WriteError(int WindowErr, int UserErr);
	
	char RecvPost(Session& session);
	char SendPost(Session& session);

	UINT Worker();
	static UINT __stdcall WorkerThread(LPVOID pLanClient);

	UINT Reconnecter();
	static UINT __stdcall ReconnecterThread(LPVOID pLanClient);

	bool LanClientOptionParsing(const WCHAR* szOptionFileName);

	bool ReleaseSession(Session& session);

private:
	UINT64 GetChannelId();
	UINT64 GetFixedChannelId(UINT64 fixedId);

private:
	BYTE		m_byNumOfWorkerThread;
	BYTE		m_byNumOfUsingWorkerThread;
	bool		m_bIsNagleOn;
	bool		m_bIsReconnect;
	UINT		m_numOfReconnect;
	WORD		m_wPort;
	BYTE		m_byNumOfSession;

	WCHAR		m_IP[16];

	HANDLE		*m_pWorkerThreadHandle;
	HANDLE		m_hWorkerIOCP;
	HANDLE		m_hReconnecterHandle;

	std::atomic<UINT64> m_sessionIdGenerator = 0;
	std::atomic<UINT64> m_channelSelector = 0;

	// sessionList라고 명명했지만, 데이터 채널이 더 맞는 표현인듯 하다
	// 일단 sessionList도 틀린 표현은 아니므로 이대로 진행
	std::vector<Session*> sessionList;

	CRITICAL_SECTION reconnectListLock;
	std::list<UINT64> sessionReconnectIdList;

	bool stopClient = false;
};
