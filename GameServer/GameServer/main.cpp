#include "PreCompile.h"
#include "RIOServer.h"
#include "PacketManager.h"
#include "DeadlockChecker.h"
#include "DBClient.h"
#include "JsonUtil.h"

int main()
{
	if (JsonUtil::LoadAllDataScript() == false)
	{
		std::cout << "Failed to load data" << std::endl;
		return 0;
	}

	{
		PacketManager::GetInst().Init();
	}
	DBClient::GetInstance().Start(L"RIO_CoreOptionFile/DBClientOptionFile.txt");

	RIOServer& server = RIOServer::GetInst();
	server.StartServer(L"RIO_CoreOptionFile/ServerOption.txt");

	while (true)
	{
		Sleep(1000);

		std::cout << server.GetSessionCount() << std::endl;

		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		{
			server.StopServer();
			break;
		}
	}
	server.StopServer();

	return 0;
}