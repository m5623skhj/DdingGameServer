#include "PreCompile.h"
#include "PC.h"
#include "GlobalUtil.h"

void PC::UpdateByPing()
{
	// management for update tick
	FireLazyRunnerIfConditionMet();
}

void PC::DBJobLazyRunnerPush(DBJobLazyRunner&& lazyRunnerInst)
{
	lazyRunnerHolder.push_back(lazyRunnerInst);
}

void PC::FireLazyRunnerIfConditionMet()
{
	for (auto& lazyRunner : lazyRunnerHolder)
	{
		if (not lazyRunner.CheckFireState())
		{
			continue;
		}

		const auto& errorCode = lazyRunner.Fire();
		if (not IsSuccess(errorCode))
		{
			// logging ?
		}
	}
}