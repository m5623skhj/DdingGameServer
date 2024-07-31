#pragma once
#include "EnumType.h"
#include <vector>

class BatchedDBJob;

class DBJobLazyRunner
{
public:
	DBJobLazyRunner(std::shared_ptr<BatchedDBJob> inOwner);
	virtual ~DBJobLazyRunner() = default;
	
public:
	virtual bool CheckFireState(const UINT64 comparison) const = 0;
	ERROR_CODE Fire();
	std::vector<std::string_view> GetBatchedDBJobName() const;

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
	FORCEINLINE virtual bool CheckFireState(const UINT64 comparisonAccumulateCount) const override;

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
	FORCEINLINE virtual bool CheckFireState(const UINT64 now) const override;

public:
	FORCEINLINE bool operator< (const DBJobTimeLazyRunner& other) const
	{
		return minFireTime < other.minFireTime;
	}

private:
	UINT64 beginTime = -1;
	UINT64 delayMillisecond = -1;
	UINT64 minFireTime = -1;
};