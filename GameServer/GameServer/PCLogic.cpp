#include "PreCompile.h"
#include "PC.h"
#include "GlobalUtil.h"

void PC::UpdateByPing()
{
	const auto& now = std::chrono::system_clock::now();
	const auto& duration = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());
	UINT64 nowMillisec = duration.count();

	// management for update tick
	FireLazyRunnerIfConditionMet(nowMillisec);
}

void PC::DBJobLazyRunnerPush(DBJobTimeLazyRunner&& lazyRunnerInst)
{
	timeLazyRunnerHolder.push(lazyRunnerInst);
}

void PC::FireLazyRunnerIfConditionMet(const UINT64 now)
{
	while (not timeLazyRunnerHolder.empty())
	{
		auto lazyRunner = timeLazyRunnerHolder.top();
		if (not lazyRunner.CheckFireState(now))
		{
			break;
		}
		timeLazyRunnerHolder.pop();

		if (not IsSuccess(lazyRunner.Fire()))
		{
			// logging?
		}
	}
}