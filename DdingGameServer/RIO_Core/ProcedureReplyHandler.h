#pragma once
#include <memory>
#include "Protocol.h"
#include <unordered_map>
#include <any>

class CSerializationBuf;
class CNetServerSerializationBuf;
class IPacket;

using DBPacketReplyHandler = std::function<bool(std::any&, OUT CSerializationBuf& recvPacket)>;

class ProcedureReplyHandler
{
private:
	ProcedureReplyHandler() = default;
	~ProcedureReplyHandler() = default;

public:
	void Initialize();
	static ProcedureReplyHandler& GetInst()
	{
		static ProcedureReplyHandler instance;
		return instance;
	}

	DECLARE_ALL_DB_REPLY_HANDLER();

public:
	template <typename PacketType>
	void RegisterPacketHandler()
	{
		static_assert(std::is_base_of<IPacket, PacketType>::value, "RegisterPacketHandler() : PacketType must inherit from IPacket");
		auto handler = [](std::any& packet, OUT CSerializationBuf& recvPacket)
		{
			auto realPacket = static_cast<PacketType*>(std::any_cast<IPacket*>(packet));
			return AssemblePacket(*realPacket, recvPacket);
		};

		PacketType packetType;
		packetHandlerMap[packetType.GetPacketId()] = handler;
	}

	void SPReplyHandle(UINT packetId, OUT CSerializationBuf& recvPacket);
	DBPacketReplyHandler GetPacketHandler(UINT packetId);

private:
	std::unordered_map<UINT, DBPacketReplyHandler> packetHandlerMap;
};