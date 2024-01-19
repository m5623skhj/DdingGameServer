#pragma once

namespace AuthProtocol
{
	enum
	{
#pragma region C2Auth
		/////////////////////////////////////
		// LoginRequest
		// {
		//		WORD packetType;
		//		char[10] id;
		//		char[20] password;
		// }
		/////////////////////////////////////
		C2Auth_LoginRequest = 10000,
#pragma endregion C2Auth

#pragma region Auth2C
		/////////////////////////////////////
		// LoginResponse
		// {
		//		WORD packetType;
		//		bool isSuccess;
		//		WCHAR characterNameList[5][20];
		//		char gameServerIP[16];
		//		WORD port;
		// }
		/////////////////////////////////////
		Auth2C_LoginResponse = 20000,
#pragma endregion Auth2C
	};
}
