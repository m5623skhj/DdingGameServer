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
}

void DBJobAccumulateLazyRunner::CheckFireState()
{
	// TODO : Check fire state
}

void DBJobTimeLazyRunner::CheckFireState()
{
	// TODO : Check fire state
}