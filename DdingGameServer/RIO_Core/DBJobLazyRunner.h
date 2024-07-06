#pragma once
#include "EnumType.h"

class BatchedDBJob;

class DBJobLazyRunner
{
public:
	DBJobLazyRunner(std::shared_ptr<BatchedDBJob> inOwner);
	virtual ~DBJobLazyRunner() = default;
	
public:
	virtual void CheckFireState() = 0;
	ERROR_CODE Fire();

protected:
	std::shared_ptr<BatchedDBJob> batchedDBJob;
};

class DBJobAccumulateLazyRunner : public DBJobLazyRunner
{
public:
	DBJobAccumulateLazyRunner() = delete;
	explicit DBJobAccumulateLazyRunner(std::shared_ptr<BatchedDBJob> inOwner, UINT inAccumulateCount);
	~DBJobAccumulateLazyRunner() override = default;

public:
	virtual void CheckFireState() override;

private:
	UINT accumulateCount = -1;
};

class DBJobTimeLazyRunner : public DBJobLazyRunner
{
public:
	DBJobTimeLazyRunner() = delete;
	explicit DBJobTimeLazyRunner(std::shared_ptr<BatchedDBJob> inOwner, UINT64 inBeginTime, UINT64 inDelayMillisecond);
	~DBJobTimeLazyRunner() override = default;

public:
	virtual void CheckFireState() override;

private:
	UINT64 beginTime = -1;
	UINT64 delayMillisecond = -1;
};