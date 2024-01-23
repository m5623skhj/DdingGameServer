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
	// ������ Connect �� �Ϸ� �� ��
	virtual void OnConnectionComplete();

	// ��Ŷ ���� �Ϸ� ��
	virtual void OnRecv(CSerializationBuf* OutReadBuf);
	// ��Ŷ �۽� �Ϸ� ��
	virtual void OnSend();

	// ��Ŀ������ GQCS �ٷ� �ϴܿ��� ȣ��
	virtual void OnWorkerThreadBegin();
	// ��Ŀ������ 1���� ���� ��
	virtual void OnWorkerThreadEnd();
	// ����� ���� ó�� �Լ�
	virtual void OnError(st_Error* OutError);
	// �� ������ �������� ����� ȣ��
	virtual void OnDisconnect();

private:

};
