#include "PreCompile.h"
#include "AuthDBThread.h"
#include "Log.h"

void AuthDBThreadManager::NotNeedDBResultHandler()
{

}

void AuthDBThreadManager::NeedDBResultHandler(DBConnection& conn, const DBJobObject& jobObject)
{
	switch (jobObject.queryObject.queryType)
	{
	case QueryType::InvalidQueryType:
	default:
		std::cout << "Invalid query type. Check query type" << std::endl;
		_LOG(LOG_LEVEL::LOG_ERROR, L"Query Fail", L"QueryType : %d / Query : %ls / SessionId : %lld / ClientType : %d"
			, jobObject.queryObject.queryType, jobObject.queryObject.query, jobObject.sessionId, jobObject.clientType);

		return;
	}

	_LOG(LOG_LEVEL::LOG_DEBUG, L"Query Success", L"QueryType : %d / Query : %ls / SessionId : %lld / ClientType : %d"
		, jobObject.queryObject.queryType, jobObject.queryObject.query, jobObject.sessionId, jobObject.clientType);
}

DBJobHandler AuthDBThreadManager::GetDBJobHandler(QueryType queryType)
{
	auto findIter = dbJobHandlerMap.find(queryType);
	if (findIter == dbJobHandlerMap.end())
	{
		return nullptr;
	}

	return findIter->second;
}
