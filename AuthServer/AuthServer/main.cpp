#include "PreCompile.h"
#include <iostream>
#include "AuthServer.h"

int main()
{
	const std::wstring folderPath = L"AuthServerOptionFile/";

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