#pragma once
#include "nlohmann/json.hpp"
#include "EnumType.h"

class Logger;

#define OBJECT_TO_JSON_LOG(...)\
    virtual nlohmann::json ObjectToJson() override {\
        nlohmann::json jsonObject;\
        { __VA_ARGS__ }\
        return jsonObject;\
    }\

#define SET_LOG_ITEM(logObject){\
    jsonObject[#logObject] = logObject;\
}

class LogBase
{
	friend Logger;

public:
	LogBase() = default;
	virtual ~LogBase() = default;

public:
	virtual nlohmann::json ObjectToJson() = 0;

private:
	nlohmann::json ObjectToJsonImpl();
	void SetLogTime();

public:
	void SetLastErrorCode(DWORD inLastErrorCode)
	{
		lastErrorCode = inLastErrorCode;
	}

private:
	std::string loggingTime;
	DWORD lastErrorCode = 0;
};

class ServerLog : public LogBase
{
public :
	OBJECT_TO_JSON_LOG(
		SET_LOG_ITEM(logString);
	);

public:
	std::string logString;
};

#define SERVER_LOG(x) ServerLog log; log.logString = x;

#define LOG_VERBOSE(x) SERVER_LOG(x)
#define LOG_DEBUG(x) if ( Logger::GetInstance().GetLogLevel() > LOG_LEVEL::Debug ) { SERVER_LOG(x) }
#define LOG_INFO(x) if ( Logger::GetInstance().GetLogLevel() > LOG_LEVEL::Info ) { SERVER_LOG(x) }
#define LOG_WARNING(x) if ( Logger::GetInstance().GetLogLevel() > LOG_LEVEL::Warning ) { SERVER_LOG(x) }
#define LOG_ERROR(x) if ( Logger::GetInstance().GetLogLevel() > LOG_LEVEL::Error ) { SERVER_LOG(x) }
#define LOG_FATAL(x) if ( Logger::GetInstance().GetLogLevel() > LOG_LEVEL::Fatal ) { SERVER_LOG(x) }