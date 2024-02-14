#include "PreCompile.h"
#include "AuthDBThread.h"

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
		break;
	}
}