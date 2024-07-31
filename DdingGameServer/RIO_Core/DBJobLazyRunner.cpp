#include "PreCompile.h"
#include "DBJobLazyRunner.h"
#include "AccumulatableDBJob.h"
#include "DBJobUtil.h"

DBJobLazyRunner::DBJobLazyRunner(std::shared_ptr<BatchedDBJob> inbatchedDBJob)
	: batchedDBJob(inbatchedDBJob)
{
}

ERROR_CODE DBJobLazyRunner::Fire()
{
	return batchedDBJob->ExecuteBatchJob();
}

std::vector<std::string_view> DBJobLazyRunner::GetBatchedDBJobName() const
{
	return batchedDBJob->GetJobNameList();
}

DBJobAccumulateLazyRunner::DBJobAccumulateLazyRunner(std::shared_ptr<BatchedDBJob> inOwner, UINT inAccumulateCount)
	: DBJobLazyRunner(inOwner)
	, accumulateCount(inAccumulateCount)
{
}

DBJobTimeLazyRunner::DBJobTimeLazyRunner(std::shared_ptr<BatchedDBJob> inOwner, UINT64 inBeginTime, UINT64 inDelayMillisecond)
	: DBJobLazyRunner(inOwner)
	, beginTime(inBeginTime)
	, delayMillisecond(inDelayMillisecond)
{
	minFireTime = beginTime + delayMillisecond;
}

bool DBJobAccumulateLazyRunner::CheckFireState(const UINT64 comparisonAccumulateCount) const
{
	return accumulateCount > comparisonAccumulateCount;
}

bool DBJobTimeLazyRunner::CheckFireState(const UINT64 now) const
{
	return minFireTime > now;
}
