#pragma once
#include "Protocol.h"

#pragma region PacketHandler
#if USING_EXTERNAL_PROTOCOL == 1

#define DECLARE_ALL_HANDLER()\
	DECLARE_HANDLE_PACKET(Ping)

#define REGISTER_PACKET_LIST()\
	REGISTER_PACKET(Ping)

#endif
#pragma endregion PacketHandler

#pragma region ForDB
#if USING_EXTERNAL_PROTOCOL == 1

#define REGISTER_ALL_DB_REPLY_HANDLER()\
	REGISTER_DB_REPLY_HANDLER(DBJobReply)

#define DECLARE_ALL_DB_REPLY_HANDLER()\
	DECLARE_DB_REPLY_HANDLER(DBJobReply)

#endif
#pragma endregion ForDB