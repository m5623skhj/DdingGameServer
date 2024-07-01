#pragma once

class DBJobLazyRunner
{
public:
	virtual ~DBJobLazyRunner() = default;
	
public:
	virtual void CheckFireState() = 0;
	void Fire();
};

class DBJobAccumulateLazyRunner : public DBJobLazyRunner
{
public:
	DBJobAccumulateLazyRunner() = delete;
	explicit DBJobAccumulateLazyRunner(UINT inAccumulateCount);
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
	explicit DBJobTimeLazyRunner(UINT64 inBeginTime, UINT64 inDelayMillisecond);
	~DBJobTimeLazyRunner() override = default;

public:
	virtual void CheckFireState() override;

private:
	UINT64 beginTime = -1;
	UINT64 delayMillisecond = -1;
};