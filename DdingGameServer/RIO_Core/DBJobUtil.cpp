#include "PreCompile.h"
#include "DBJobUtil.h"

std::shared_ptr<BatchedDBJob> MakeBatchedDBJob(RIOSession& inOwner)
{
	auto returnObject = std::make_shared<BatchedDBJob>(inOwner);
	DBJobManager::GetInst().RegisterDBJob(returnObject);

	return returnObject;
}