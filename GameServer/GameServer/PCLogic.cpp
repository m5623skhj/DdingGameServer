#include "PreCompile.h"
#include "PC.h"
#include "GlobalUtil.h"
#include "Logger.h"

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

		if (auto error = lazyRunner.Fire(); not IsSuccess(error))
		{
			std::ostringstream logStream;
			logStream << "LazyRunner fire failed in PC tick. Error "
				<< static_cast<UINT>(error)
				<< ". DB job list: ";

			const auto jobNameList = lazyRunner.GetBatchedDBJobName();
			for (const auto& item : jobNameList)
			{
				logStream << item << " / ";
			}

			auto log = LogHelper::MakeLogObject<ServerLog>();
			log->logString = logStream.str();
			
			Logger::GetInstance().WriteLog(log);
		}
	}
}