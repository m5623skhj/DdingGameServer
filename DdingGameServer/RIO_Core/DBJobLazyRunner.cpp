#include "PreCompile.h"
#include "DBJobLazyRunner.h"

void DBJobLazyRunner::Fire()
{
	// TODO : Fire db job
}

DBJobAccumulateLazyRunner::DBJobAccumulateLazyRunner(UINT inAccumulateCount)
	: accumulateCount(inAccumulateCount)
{
}


DBJobTimeLazyRunner::DBJobTimeLazyRunner(UINT64 inBeginTime, UINT64 inDelayMillisecond)
	: beginTime(inBeginTime)
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