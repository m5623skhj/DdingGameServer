#include "PreCompile.h"
#include <iostream>
#include "AuthServer.h"
#include "AuthDBConnector.h"

int main()
{
	const std::wstring folderPath = L"AuthServerOptionFile/";

	AuthDBConnector& connector = AuthDBConnector::GetInst();
	if (connector.ConnectDB(folderPath + L"DBConnectFile.txt") == false)
	{
		std::cout << "---------------------" << std::endl;
		std::cout << "ConnectDB() failed" << std::endl;
		std::cout << "---------------------" << std::endl << std::endl << std::endl;

		return 0;
	}

	if (AuthServer::GetInst().StartAuthServer(folderPath + L"LanServerOptionFile.txt", folderPath + L"NetServerOptionFile.txt") == false)
	{
		std::cout << "Auth server start failed" << std::endl;
		return 0;
	}
	std::cout << "Auth server running" << std::endl;

	while (1)
	{
		// do what?
	}

	return 0;
}