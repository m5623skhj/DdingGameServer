#include "PreCompile.h"
#pragma comment(lib, "ws2_32")
#include "MultiLanClient.h"
#include "CrashDump.h"
#include "LanServerSerializeBuf.h"
#include "Profiler.h"
#include "Log.h"

CMultiLanClient::CMultiLanClient()
{
	InitializeCriticalSection(&reconnectListLock);
}

CMultiLanClient::~CMultiLanClient()
{

}

bool CMultiLanClient::Start(const WCHAR *szOptionFileName)
{
	if (!LanClientOptionParsing(szOptionFileName))
	{
		WriteError(WSAGetLastError(), SERVER_ERR::PARSING_ERR);
		return false;
	}

	for (BYTE i = 0; i < m_byNumOfSession; ++i)
	{
		sessionList.emplace_back(new Session());
	}

	int retval;
	WSADATA Wsa;
	if (WSAStartup(MAKEWORD(2, 2), &Wsa))
	{
		WriteError(WSAGetLastError(), SERVER_ERR::WSASTARTUP_ERR);
		return false;
	}

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	InetPton(AF_INET, m_IP, &serveraddr.sin_addr);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(m_wPort);

	m_pWorkerThreadHandle = new HANDLE[m_byNumOfWorkerThread];

	m_hWorkerIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, m_byNumOfWorkerThread);
	if (m_hWorkerIOCP == NULL)
	{
		WriteError(WSAGetLastError(), SERVER_ERR::WORKERIOCP_NULL_ERR);
		return false;
	}

	// static 함수에서 LanClient 객체를 접근하기 위하여 this 포인터를 인자로 넘김
	for (int i = 0; i < m_byNumOfWorkerThread; i++)
		m_pWorkerThreadHandle[i] = (HANDLE)_beginthreadex(NULL, 0, WorkerThread, this, 0, NULL);
	if (m_pWorkerThreadHandle == 0)
	{
		WriteError(WSAGetLastError(), SERVER_ERR::BEGINTHREAD_ERR);
		return false;
	}
	m_byNumOfWorkerThread = m_byNumOfWorkerThread;
	m_hReconnecterHandle = (HANDLE)_beginthreadex(NULL, 0, ReconnecterThread, this, 0, NULL);

	m_numOfReconnect = 0;
	
	for (auto& session : sessionList)
	{
		session->m_sock = socket(AF_INET, SOCK_STREAM, 0);
		if (session->m_sock == INVALID_SOCKET)
		{
			WriteError(WSAGetLastError(), SERVER_ERR::LISTEN_SOCKET_ERR);
			return false;
		}

		while (1)
		{
			if (connect(session->m_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr)) != SOCKET_ERROR)
				break;
			if (GetLastError() == WSAEISCONN)
			{
				closesocket(session->m_sock);

				session->m_sock = socket(AF_INET, SOCK_STREAM, 0);
				if (session->m_sock == INVALID_SOCKET)
				{
					WriteError(WSAGetLastError(), SERVER_ERR::LISTEN_SOCKET_ERR);
					return false;
				}
			}

			Sleep(1000);
		}

		session->bIsConnect = true;

		retval = setsockopt(session->m_sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&m_bIsNagleOn, sizeof(int));
		if (retval == SOCKET_ERROR)
		{
			WriteError(WSAGetLastError(), SERVER_ERR::SETSOCKOPT_ERR);
			return false;
		}

		ZeroMemory(&session->m_RecvIOData.Overlapped, sizeof(OVERLAPPED));
		ZeroMemory(&session->m_SendIOData.Overlapped, sizeof(OVERLAPPED));

		CreateIoCompletionPort((HANDLE)session->m_sock, m_hWorkerIOCP, (ULONG_PTR)&session, 0);

		session->m_SendIOData.lBufferCount = 0;
		session->m_SendIOData.IOMode = NONSENDING;

		session->m_SessionId = m_sessionIdGenerator;
		++m_sessionIdGenerator;

		RecvPost(*session);
		OnConnectionComplete();
	}

	return true;
}

void CMultiLanClient::Stop()
{
	stopClient = true;

	for (auto& session : sessionList)
	{
		shutdown(session->m_sock, SD_BOTH);
	}

	PostQueuedCompletionStatus(m_hWorkerIOCP, 0, 0, (LPOVERLAPPED)1);
	WaitForMultipleObjects(m_byNumOfWorkerThread, m_pWorkerThreadHandle, TRUE, INFINITE);

	for (BYTE i = 0; i < m_byNumOfWorkerThread; ++i)
		CloseHandle(m_pWorkerThreadHandle[i]);
	CloseHandle(m_hWorkerIOCP);

	WSACleanup();
}

bool CMultiLanClient::LanClientOptionParsing(const WCHAR *szOptionFileName)
{
	_wsetlocale(LC_ALL, L"Korean");

	CParser parser;
	WCHAR cBuffer[BUFFER_MAX];

	FILE *fp;
	_wfopen_s(&fp, szOptionFileName, L"rt, ccs=UNICODE");
	if (fp == NULL)
		return false;

	int iJumpBOM = ftell(fp);
	fseek(fp, 0, SEEK_END);
	int iFileSize = ftell(fp);
	fseek(fp, iJumpBOM, SEEK_SET);
	int FileSize = (int)fread_s(cBuffer, BUFFER_MAX, sizeof(WCHAR), BUFFER_MAX / 2, fp);
	int iAmend = iFileSize - FileSize; // 개행 문자와 파일 사이즈에 대한 보정값
	fclose(fp);

	cBuffer[iFileSize - iAmend] = '\0';
	WCHAR *pBuff = cBuffer;

	if (!parser.GetValue_String(pBuff, L"LANCLIENT", L"SERVER_IP", m_IP))
		return false;
	if (!parser.GetValue_Short(pBuff, L"LANCLIENT", L"SERVER_PORT", (short*)&m_wPort))
		return false;

	m_byNumOfWorkerThread = 1;
	m_byNumOfUsingWorkerThread = 1;
	m_byNumOfSession = 1;

	if (!parser.GetValue_Byte(pBuff, L"LANCLIENT", L"WORKER_THREAD", &m_byNumOfWorkerThread))
		return false;
	if (!parser.GetValue_Byte(pBuff, L"LANCLIENT", L"USE_IOCPWORKER", &m_byNumOfUsingWorkerThread))
		return false;
	if (!parser.GetValue_Byte(pBuff, L"LANCLIENT", L"NAGLE_ON", (BYTE*)&m_bIsNagleOn))
		return false;
	if (!parser.GetValue_Byte(pBuff, L"LANCLIENT", L"RECONNECT", (BYTE*)&m_bIsReconnect))
		return false;
	if (!parser.GetValue_Byte(pBuff, L"LANCLIENT", L"SESSION_COUNT", (BYTE*)&m_byNumOfSession))
		return false;

	return true;
}

bool CMultiLanClient::ReleaseSession(Session& session)
{
	OnDisconnect();
	session.bIsConnect = false;

	int SendBufferRestSize = session.m_SendIOData.lBufferCount;
	int Rest = session.m_SendIOData.SendQ.GetRestSize();
	// SendPost 에서 옮겨졌으나 보내지 못한 직렬화 버퍼들을 반환함
	for (int i = 0; i < SendBufferRestSize; ++i)
		CSerializationBuf::Free(session.pSeirializeBufStore[i]);

	// 큐에서 아직 꺼내오지 못한 직렬화 버퍼가 있다면 해당 직렬화 버퍼들을 반환함
	if (Rest > 0)
	{
		CSerializationBuf *DeleteBuf;
		for (int i = 0; i < Rest; ++i)
		{
			session.m_SendIOData.SendQ.Dequeue(&DeleteBuf);
			CSerializationBuf::Free(DeleteBuf);
		}
	}

	closesocket(session.m_sock);
	if (!m_bIsReconnect)
		return true;

	session.m_sock = INVALID_SOCKET;

	EnterCriticalSection(&reconnectListLock);

	sessionReconnectIdList.emplace_back(session.m_SessionId);

	LeaveCriticalSection(&reconnectListLock);

	return true;
}

UINT CMultiLanClient::Worker()
{
	char cPostRetval;
	bool GQCSSuccess;
	int retval;
	DWORD Transferred;
	Session** pSession;
	LPOVERLAPPED lpOverlapped;
	srand((UINT)&retval);

	while (1)
	{
		cPostRetval = -1;
		Transferred = 0;
		pSession = NULL;
		lpOverlapped = NULL;

		GQCSSuccess = GetQueuedCompletionStatus(m_hWorkerIOCP, &Transferred, (PULONG_PTR)&pSession, &lpOverlapped, INFINITE);
		OnWorkerThreadBegin();
		// GQCS 에 완료 통지가 왔을 경우
		if (lpOverlapped != NULL)
		{
			// 외부 종료코드에 의한 종료
			if (pSession == NULL)
			{
				PostQueuedCompletionStatus(m_hWorkerIOCP, 0, 0, NULL);
				break;
			}

			auto session = *pSession;
			if (session == nullptr)
			{
				g_Dump.Crash();
			}
			// recv 파트
			if (lpOverlapped == &session->m_RecvIOData.Overlapped)
			{
				// 서버가 종료함
				// 재접속 필요
				if (Transferred == 0)
				{
					// 현재 m_IOCount를 줄여서 이전값이 1일경우 해당 세션을 삭제함
					if (InterlockedDecrement(&session->m_IOCount) == 0)
						ReleaseSession(*session);

					continue;
				}

				session->m_RecvIOData.RingBuffer.MoveWritePos(Transferred);
				int RingBufferRestSize = session->m_RecvIOData.RingBuffer.GetUseSize();

				while (RingBufferRestSize > HEADER_SIZE)
				{
					CSerializationBuf &RecvSerializeBuf = *CSerializationBuf::Alloc();
					RecvSerializeBuf.m_iRead = 0;
					retval = session->m_RecvIOData.RingBuffer.Peek((char*)RecvSerializeBuf.m_pSerializeBuffer, HEADER_SIZE);
					if (retval < HEADER_SIZE)
					{
						CSerializationBuf::Free(&RecvSerializeBuf);
						break;
					}
					// PayloadLength 2
					WORD PayloadLength;
					RecvSerializeBuf >> PayloadLength;

					if (session->m_RecvIOData.RingBuffer.GetUseSize() < PayloadLength + HEADER_SIZE)
					{
						CSerializationBuf::Free(&RecvSerializeBuf);
						break;
					}
					session->m_RecvIOData.RingBuffer.RemoveData(retval);

					retval = session->m_RecvIOData.RingBuffer.Dequeue(&RecvSerializeBuf.m_pSerializeBuffer[RecvSerializeBuf.m_iWrite], PayloadLength);
					RecvSerializeBuf.m_iWrite += retval;

					RingBufferRestSize -= (retval + sizeof(WORD));
					OnRecv(&RecvSerializeBuf);
					CSerializationBuf::Free(&RecvSerializeBuf);
				}

				cPostRetval = RecvPost(*session);
			}
			// send 파트
			else if (lpOverlapped == &session->m_SendIOData.Overlapped)
			{
				int BufferCount = session->m_SendIOData.lBufferCount;
				for (int i = 0; i < BufferCount; ++i)
				{
					CSerializationBuf::Free(session->pSeirializeBufStore[i]);
				}

				session->m_SendIOData.lBufferCount -= BufferCount;

				OnSend();
				InterlockedExchange(&session->m_SendIOData.IOMode, NONSENDING); // 여기 다시 생각해 볼 것
				cPostRetval = SendPost(*session);
			}
		}
		else
		{
			st_Error Error;
			Error.GetLastErr = WSAGetLastError();
			Error.ServerErr = SERVER_ERR::OVERLAPPED_NULL_ERR;
			OnError(&Error);

			g_Dump.Crash();
		}

		OnWorkerThreadEnd();

		if (cPostRetval == POST_RETVAL_ERR_SESSION_DELETED)
			continue;
		if (InterlockedDecrement(&(*pSession)->m_IOCount) == 0)
			ReleaseSession(**pSession);
	}

	CSerializationBuf::ChunkFreeForcibly();
	return 0;
}

UINT __stdcall CMultiLanClient::WorkerThread(LPVOID pLanClient)
{
	return ((CMultiLanClient*)pLanClient)->Worker();
}

UINT CMultiLanClient::Reconnecter()
{
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	InetPton(AF_INET, m_IP, &serveraddr.sin_addr);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(m_wPort);

	while (1)
	{
		Sleep(5000);

		if (stopClient == true)
		{
			return 0;
		}

		EnterCriticalSection(&reconnectListLock);

		if (sessionReconnectIdList.empty() == true)
		{
			LeaveCriticalSection(&reconnectListLock);
			continue;
		}
			
		UINT64 releasedSessionId = *sessionReconnectIdList.begin();
		LeaveCriticalSection(&reconnectListLock);

		sessionList[releasedSessionId]->m_sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sessionList[releasedSessionId]->m_sock == INVALID_SOCKET)
		{
			WriteError(WSAGetLastError(), SERVER_ERR::LISTEN_SOCKET_ERR);
			return -1;
		}

		_LOG(LOG_LEVEL::LOG_WARNING, L"LanClient", L"ReConnection %d / ThreadID : %d", m_numOfReconnect, GetCurrentThreadId());

		while (1)
		{
			if (stopClient == true)
			{
				return 0;
			}

			if (connect(sessionList[releasedSessionId]->m_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr)) != SOCKET_ERROR)
				break;
			if (GetLastError() == WSAEISCONN)
			{
				closesocket(sessionList[releasedSessionId]->m_sock);

				sessionList[releasedSessionId]->m_sock = socket(AF_INET, SOCK_STREAM, 0);
				if (sessionList[releasedSessionId]->m_sock == INVALID_SOCKET)
				{
					WriteError(WSAGetLastError(), SERVER_ERR::LISTEN_SOCKET_ERR);
					return -1;
				}
			}

			Sleep(1000);
		}

		sessionList[releasedSessionId]->bIsConnect = true;

		int retval = setsockopt(sessionList[releasedSessionId]->m_sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&m_bIsNagleOn, sizeof(int));
		if (retval == SOCKET_ERROR)
		{
			WriteError(WSAGetLastError(), SERVER_ERR::SETSOCKOPT_ERR);
			return -1;
		}

		CreateIoCompletionPort((HANDLE)sessionList[releasedSessionId]->m_sock, m_hWorkerIOCP, (ULONG_PTR)&sessionList[releasedSessionId], 0);

		RecvPost(*sessionList[releasedSessionId]);

		++m_numOfReconnect;
		OnConnectionComplete();

		sessionReconnectIdList.pop_front();
	}

	return 0;
}

UINT __stdcall CMultiLanClient::ReconnecterThread(LPVOID pLanClient)
{
	return ((CMultiLanClient*)pLanClient)->Reconnecter();
}

// 해당 함수가 정상완료 및 pSession이 해제되지 않았으면 true 그 외에는 false를 반환함
char CMultiLanClient::RecvPost(Session& session)
{
	OVERLAPPEDIODATA &RecvIOData = session.m_RecvIOData;
	if (RecvIOData.RingBuffer.IsFull())
	{
		// 현재 m_IOCount를 줄여서 이전값이 1일경우 해당 세션을 삭제함
		if (InterlockedDecrement(&session.m_IOCount) == 0)
		{
			ReleaseSession(session);
			return POST_RETVAL_ERR_SESSION_DELETED;
		}
		return POST_RETVAL_ERR;
	}

	int BrokenSize = RecvIOData.RingBuffer.GetNotBrokenPutSize();
	int RestSize = RecvIOData.RingBuffer.GetFreeSize() - BrokenSize;
	int BufCount = 1;
	DWORD Flag = 0;

	WSABUF wsaBuf[2];
	wsaBuf[0].buf = RecvIOData.RingBuffer.GetWriteBufferPtr();
	wsaBuf[0].len = BrokenSize;
	if (RestSize > 0)
	{
		wsaBuf[1].buf = RecvIOData.RingBuffer.GetBufferPtr();
		wsaBuf[1].len = RestSize;
		BufCount++;
	}
	InterlockedIncrement(&session.m_IOCount);
	int retval = WSARecv(session.m_sock, wsaBuf, BufCount, NULL, &Flag, &RecvIOData.Overlapped, 0);
	if (retval == SOCKET_ERROR)
	{
		int GetLastErr = WSAGetLastError();
		if (GetLastErr != ERROR_IO_PENDING)
		{
			// 현재 m_IOCount를 줄여서 이전값이 1일경우 해당 세션을 삭제함
			if (InterlockedDecrement(&session.m_IOCount) == 0)
			{
				ReleaseSession(session);
				return POST_RETVAL_ERR_SESSION_DELETED;
			}
			st_Error Error;
			Error.GetLastErr = GetLastErr;
			Error.ServerErr = SERVER_ERR::WSARECV_ERR;
			OnError(&Error);
			return POST_RETVAL_ERR;
		}
	}
	return POST_RETVAL_COMPLETE;
}

UINT64 CMultiLanClient::GetChannelId()
{
	return ++m_channelSelector % m_byNumOfWorkerThread;
}

UINT64 CMultiLanClient::GetFixedChannelId(UINT64 fixedId)
{
	return fixedId % m_byNumOfUsingWorkerThread;
}

bool CMultiLanClient::SendPacket(CSerializationBuf *pSerializeBuf)
{
	return SendPacketImpl(pSerializeBuf, GetChannelId());
}

bool CMultiLanClient::SendPacket(CSerializationBuf* pSerializeBuf, UINT64 fixedId)
{
	return SendPacketImpl(pSerializeBuf, GetFixedChannelId(fixedId));
}

bool CMultiLanClient::SendPacketImpl(CSerializationBuf* pSerializeBuf, UINT64 channelId)
{
	if (pSerializeBuf == NULL)
	{
		st_Error Error;
		Error.GetLastErr = WSAGetLastError();
		Error.ServerErr = SERVER_ERR::SERIALIZEBUF_NULL_ERR;
		OnError(&Error);
		return false;
	}

	WORD PayloadSize = pSerializeBuf->GetUseSize();
	pSerializeBuf->m_iWriteLast = pSerializeBuf->m_iWrite;
	pSerializeBuf->m_iWrite = 0;
	pSerializeBuf->m_iRead = 0;
	*pSerializeBuf << PayloadSize;

	// session에 대한 락을 좀 생각 해봐야할듯
	sessionList[channelId]->m_SendIOData.SendQ.Enqueue(pSerializeBuf);
	SendPost(*sessionList[channelId]);

	return true;
}

// 해당 함수가 정상완료 및 pSession이 해제되지 않았으면 true 그 외에는 false를 반환함
char CMultiLanClient::SendPost(Session& session)
{
	OVERLAPPED_SEND_IO_DATA &SendIOData = session.m_SendIOData;
	while (1)
	{
		if (InterlockedCompareExchange(&SendIOData.IOMode, SENDING, NONSENDING))
			return POST_RETVAL_COMPLETE;

		int UseSize = SendIOData.SendQ.GetRestSize();
		if (UseSize == 0)
		{
			InterlockedExchange(&SendIOData.IOMode, NONSENDING);
			if (SendIOData.SendQ.GetRestSize() > 0)
				continue;
			return POST_RETVAL_COMPLETE;
		}
		else if (UseSize < 0)
		{
			if (InterlockedDecrement(&session.m_IOCount) == 0)
			{
				ReleaseSession(session);
				return POST_RETVAL_ERR_SESSION_DELETED;
			}
			InterlockedExchange(&SendIOData.IOMode, NONSENDING);
			return POST_RETVAL_ERR;
		}

		WSABUF wsaSendBuf[IOCP_ONE_SEND_WSABUF_MAX];
		if (IOCP_ONE_SEND_WSABUF_MAX < UseSize)
			UseSize = IOCP_ONE_SEND_WSABUF_MAX;

		for (int i = 0; i < UseSize; ++i)
		{
			SendIOData.SendQ.Dequeue(&session.pSeirializeBufStore[i]);
			wsaSendBuf[i].buf = session.pSeirializeBufStore[i]->GetBufferPtr();
			wsaSendBuf[i].len = session.pSeirializeBufStore[i]->GetAllUseSize();
		}
		SendIOData.lBufferCount = UseSize;

		InterlockedIncrement(&session.m_IOCount);
		int retval = WSASend(session.m_sock, wsaSendBuf, UseSize, NULL, 0, &SendIOData.Overlapped, 0);
		if (retval == SOCKET_ERROR)
		{
			int err = WSAGetLastError();
			if (err != ERROR_IO_PENDING)
			{
				if (err == WSAENOBUFS)
					g_Dump.Crash();
				st_Error Error;
				Error.GetLastErr = err;
				Error.ServerErr = SERVER_ERR::WSASEND_ERR;
				OnError(&Error);

				// 현재 m_IOCount를 줄여서 이전값이 1일경우 해당 세션을 삭제함
				if (InterlockedDecrement(&session.m_IOCount) == 0)
				{
					ReleaseSession(session);
					return POST_RETVAL_ERR_SESSION_DELETED;
				}

				return POST_RETVAL_ERR;
			}
		}
	}
	return POST_RETVAL_ERR_SESSION_DELETED;
}

void CMultiLanClient::WriteError(int WindowErr, int UserErr)
{
	st_Error Error;
	Error.GetLastErr = WindowErr;
	Error.ServerErr = UserErr;
	OnError(&Error);
}
