#include "PreCompile.h"
#include "DBServer.h"
#include "LanServerSerializeBuf.h"
#include "StoredProcedure.h"
#include "ODBCConnector.h"
#include "../../DdingGameServer/RIO_Core/Protocol.h"

using namespace std;

void DBServer::InsertBatchJob(DBJobKey jobKey, const DBJobStart& job)
{
	std::lock_guard<shared_mutex> lock(batchedDBJobMapLock);
	batchedDBJobMap.insert({ jobKey, make_shared<BatchedDBJob>(job.batchSize, job.sessionId) });
}

void DBServer::HandlePacket(const UINT64 requestSessionId, PACKET_ID packetId, CSerializationBuf* recvBuffer)
{
	DBJobKey key = INVALID_DB_JOB_KEY;
	*recvBuffer >> key;

	if (packetId == PACKET_ID::GAME2DB_BATCHED_DB_JOB)
	{
		DBJobStart job;
		*recvBuffer >> job.sessionId >> job.batchSize;
		InsertBatchJob(key, job);
	}
	else
	{
		if (key == 0)
		{
			ProcedureHandleImpl(requestSessionId, packetId, recvBuffer);
		}

		if (IsBatchJobWaitingJob(key) == true)
		{
			AddItemForJobStart(requestSessionId, key, packetId, recvBuffer);
		}
		else
		{
			std::cout << "Receive invalid job. Request session id : " << requestSessionId 
				<< " Packet id : " << static_cast<UINT>(packetId) << std::endl;
		}
	}
}

void DBServer::AddItemForJobStart(const UINT64 requestSessionId, const DBJobKey jobKey, PACKET_ID packetId, CSerializationBuf* recvBuffer)
{
	std::shared_ptr<BatchedDBJob> batchedJob = nullptr;
	{
		std::lock_guard<shared_mutex> lock(batchedDBJobMapLock);
		const auto& itor = batchedDBJobMap.find(jobKey);
		if (itor == batchedDBJobMap.end())
		{
			return;
		}

		batchedJob = itor->second;
		batchedDBJobMap.erase(jobKey);
	}

	if (batchedJob == nullptr)
	{
		return;
	}

	CSerializationBuf::AddRefCount(recvBuffer);
	batchedJob->bufferList.emplace_back(packetId, recvBuffer);
	if (batchedJob->batchSize == batchedJob->bufferList.size())
	{
		DoBatchedJob(requestSessionId, jobKey, batchedJob);
	}
}

void DBServer::DoBatchedJob(const UINT64 requestSessionId, const DBJobKey jobKey, const std::shared_ptr<BatchedDBJob>& batchedJob)
{
	bool isSuccess = true;

	ODBCConnector& connector = ODBCConnector::GetInst();
	auto conn = connector.GetConnection();
	if (conn == nullopt)
	{
		g_Dump.Crash();
	}

	if (ODBCUtil::SQLIsSuccess(
		SQLSetConnectAttr(conn.value().dbcHandle, SQL_ATTR_AUTOCOMMIT, SQL_AUTOCOMMIT_OFF, 0)) == false)
	{
		g_Dump.Crash();
	}

	for (const auto& [packetId, buffer] : batchedJob->bufferList)
	{
		auto result = DBJobHandleImpl(requestSessionId, batchedJob->sessionId, static_cast<PACKET_ID>(packetId), conn.value(), buffer);
		CSerializationBuf::Free(buffer);

		if (isSuccess == false)
		{
			break;
		}
	}

	CSerializationBuf* resultPacket = CSerializationBuf::Alloc();
	*resultPacket << jobKey << isSuccess;

	if (isSuccess == true)
	{
		if (ODBCUtil::SQLIsSuccess(SQLEndTran(SQL_HANDLE_DBC, conn.value().dbcHandle, SQL_COMMIT)) == false)
		{
			g_Dump.Crash();
		}
	}
	else
	{
		if (ODBCUtil::SQLIsSuccess(SQLEndTran(SQL_HANDLE_DBC, conn.value().dbcHandle, SQL_ROLLBACK)) == false)
		{
			g_Dump.Crash();
		}
	}
	SendPacket(requestSessionId, resultPacket);
	SQLSetConnectAttr(conn.value().dbcHandle, SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, 0);
	connector.FreeConnection(conn.value());
}

bool DBServer::IsBatchJobWaitingJob(const DBJobKey jobKey)
{
	std::shared_lock<shared_mutex> lock(batchedDBJobMapLock);
	if (const auto& itor = batchedDBJobMap.find(jobKey); itor == batchedDBJobMap.end())
	{
		return false;
	}

	return true;
}

ProcedureResult DBServer::ProcedureHandleImpl(UINT64 requestSessionId, PACKET_ID packetId, CSerializationBuf* recvBuffer)
{
	CSerializationBuf* packet = CSerializationBuf::Alloc();
	bool isSuccess = false;
	ODBCConnector& connector = ODBCConnector::GetInst();
	auto conn = connector.GetConnection();
	if (conn == nullopt)
	{
		g_Dump.Crash();
	}

	/*
	switch (packetId)
	{
	default:
		break;
	}
	*/

	connector.FreeConnection(conn.value());
	return ProcedureResult(isSuccess, packet);
}

bool DBServer::DBJobHandleImpl(UINT64 requestSessionId, UINT64 userSessionId, PACKET_ID packetId, DBConnection& conn, CSerializationBuf* recvBuffer)
{
	bool isSuccess = false;

	ODBCConnector& connector = ODBCConnector::GetInst();

	/*
	switch (packetId)
	{
	default:
		cout << "Invalid packet id : " << static_cast<UINT>(packetId) << endl;
		break;
	}
	*/

	return isSuccess;
}