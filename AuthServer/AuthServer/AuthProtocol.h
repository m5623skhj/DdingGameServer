#pragma once

namespace AuthPtorocol
{
	enum
	{
#pragma region C2Auth
		/////////////////////////////////////
		// LoginRequest
		// {
		//		char[10] Id
		//		char[20] password
		// }
		/////////////////////////////////////
		C2Auth_LoginRequest = 10000,
#pragma endregion C2Auth

#pragma region Auth2C
		/////////////////////////////////////
		// LoginResponse
		// {
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
