#pragma once
#include <list>
#include <memory>
#include "EnumType.h"
#include "DefineType.h"
#include <unordered_map>
#include <mutex>
#include "LanServerSerializeBuf.h"
#include "RIOSession.h"
#include "Protocol.h"
#include <optional>

#define DEFINE_DB_JOB_CLASS_NAME(className)\
	virtual const std::string_view GetDBJobClassName() const override {\
		return std::string_view(#className);\
	}

class RIOSession;

class DBJob
{
	friend class BatchedDBJob;

public:
	DBJob() = delete;
	template <typename T, typename = std::enable_if_t<std::is_base_of_v<IGameAndDBPacket, T>>>
	explicit DBJob(RIOSession& inOwner, T& packet, T& rollbackPacket)
		: owner(inOwner)
	{
		CSerializationBuf& buffer = *CSerializationBuf::Alloc();
		buffer << packet.GetPacketId();
		jobKeyPosition = buffer.GetWriteBufferPtr();
		buffer.MoveWritePos(sizeof(DBJobKey));
		packet.PacketToBuffer(buffer);

		forRollback = std::make_shared<T>(rollbackPacket);
	}
	virtual ~DBJob();

public:
	virtual const std::string_view GetDBJobClassName() const = 0;

public:
	virtual void OnCommit() = 0;
	virtual void OnRollback() = 0;

public:
	virtual void Clear() {}

public:
	bool ExecuteJob(DBJobKey batchDBJobKey);
	CSerializationBuf* GetJobBuffer();

private:
	RIOSession& owner;

private:
	CSerializationBuf* jobSPBuffer = nullptr;
	char* jobKeyPosition = nullptr;

protected:
	IGameAndDBPacket* GetRollbackItem();

private:
	std::shared_ptr<IGameAndDBPacket> forRollback = nullptr;
};

class BatchedDBJob
{
public:
	BatchedDBJob() = delete;
	explicit BatchedDBJob(RIOSession& inOwner);
	virtual ~BatchedDBJob();

public:
	ERROR_CODE AddDBJob(std::shared_ptr<DBJob> job);
	ERROR_CODE ExecuteBatchJob();

	void OnCommit();
	void OnRollback();

public:
	std::vector<std::string_view> GetJobNameList() const;

private:
	std::list<std::shared_ptr<DBJob>> jobList;
	RIOSession& owner;

public:
	DBJobKey GetDBJobKey() { return dbJobKey; }

private:
	DBJobKey dbJobKey = INVALID_DB_JOB_KEY;
};

class GlobalDBJob : public DBJob
{
public:
	GlobalDBJob() = default;
	virtual ~GlobalDBJob() = default;
};

class GlobalBatchedDBJob : public BatchedDBJob
{
public:
	GlobalBatchedDBJob() = default;
	virtual ~GlobalBatchedDBJob() = default;
};

class DBJobManager
{
private:
	DBJobManager() = default;
	~DBJobManager() = default;

	DBJobManager(const DBJobManager&) = delete;
	DBJobManager& operator=(const DBJobManager&) = delete;

public:
	static DBJobManager& GetInst();

public:
	void RegisterDBJob(std::shared_ptr<BatchedDBJob> job);
	std::shared_ptr<BatchedDBJob> GetRegistedDBJob(DBJobKey jobKey);
	void DeregisterDBJob(DBJobKey jobKey);

	DBJobKey GetDBJobKey();

private:
	std::atomic<DBJobKey> jobKey = 1;

	// 관리의 용이성을 위해서 일반 DBJob도 BatchedDBJob에 넣어서 보냄
	std::unordered_map<DBJobKey, std::shared_ptr<BatchedDBJob>> jobMap;
	std::mutex jobMapLock;
};