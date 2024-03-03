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
		_LOG(LOG_LEVEL::LOG_ERROR, L"Query Fail", L"Query : %ls / SessionId : %lld / ClientType : %d"
			, jobObject.queryObject, jobObject.sessionId, jobObject.clientType);

		return;
	}

	_LOG(LOG_LEVEL::LOG_DEBUG, L"Query Success", L"Query : %ls / SessionId : %lld / ClientType : %d"
		, jobObject.queryObject, jobObject.sessionId, jobObject.clientType);
}