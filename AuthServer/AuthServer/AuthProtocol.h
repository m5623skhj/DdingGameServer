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
		//		std::string id;
		//		std::string password;
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

#pragma region Auth2Game
		/////////////////////////////////////
		// RegisterSuccess
		// {
		//		bool isConnected;
		// }
		/////////////////////////////////////
		Auth2Game_RegisterSuccess = 30000,
#pragma endregion Auth2Game

#pragma region Game2Auth
		/////////////////////////////////////
		// RegisterGameServer
		// {
		//		WORD gameServerId;
		// }
		/////////////////////////////////////
		Game2Auth_Register = 40000,
#pragma endregion Game2Auth
	};
}
