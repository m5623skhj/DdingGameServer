#pragma once
#include "LanClient.h"

class AuthClient : public CLanClient
{
private:
	AuthClient();
	virtual ~AuthClient();

public:
	bool Start(const std::wstring& optionFile);

public:
	// 서버에 Connect 가 완료 된 후
	virtual void OnConnectionComplete();

	// 패킷 수신 완료 후
	virtual void OnRecv(CSerializationBuf* OutReadBuf);
	// 패킷 송신 완료 후
	virtual void OnSend();

	// 워커스레드 GQCS 바로 하단에서 호출
	virtual void OnWorkerThreadBegin();
	// 워커스레드 1루프 종료 후
	virtual void OnWorkerThreadEnd();
	// 사용자 에러 처리 함수
	virtual void OnError(st_Error* OutError);
	// 이 세션이 서버에서 끊기면 호출
	virtual void OnDisconnect();

private:

};
