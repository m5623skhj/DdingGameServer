#include "PreCompile.h"
#include "AuthDBConnector.h"

namespace LocalUtil
{
	bool SQLIsSuccess(SQLRETURN returnValue)
	{
		if (returnValue == SQL_SUCCESS || returnValue == SQL_SUCCESS_WITH_INFO)
		{
			return true;
		}

		return false;
	}
}

DBConnectionPool::~DBConnectionPool()
{
	Cleanup();
}

bool DBConnectionPool::Initialize(const std::wstring& inConnectionString, int inPoolSize)
{
	connectionString = inConnectionString;
	poolSize = inPoolSize;

	if (poolSize == 0)
	{
		poolSize = 1;
	}

	return Initialize();
}

void DBConnectionPool::Cleanup()
{
	std::lock_guard<std::mutex> lock(connectionLock);

	for (auto& connection : connectionList)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, connection.stmtHandle);
		SQLDisconnect(connection.dbcHandle);
		SQLFreeHandle(SQL_HANDLE_DBC, connection.dbcHandle);
		connectionList.pop_front();
	}
}

std::optional<DBConnection> DBConnectionPool::GetConnection()
{
	std::lock_guard<std::mutex> lock(connectionLock);

	if (connectionList.empty())
	{
		return std::nullopt;
	}

	auto& connection = connectionList.front();
	connectionList.pop_front();

	return connection;
}

void DBConnectionPool::FreeConnection(DBConnection& connection)
{
	std::lock_guard<std::mutex> lock(connectionLock);
	connectionList.emplace_back(connection);
}

bool DBConnectionPool::Initialize()
{
	SQLHENV enviromentHandle;
	SQLRETURN sqlReturn;

	sqlReturn = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &enviromentHandle);
	if (sqlReturn != SQL_SUCCESS)
	{
		std::cout << "env handle alloc Failed : " << sqlReturn << std::endl;
		return false;
	}

	sqlReturn = SQLSetEnvAttr(enviromentHandle, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	if (sqlReturn != SQL_SUCCESS)
	{
		std::cout << "SQLSetEnvAttr() Failed : " << sqlReturn << std::endl;
		return false;
	}

	for (int i = 0; i < poolSize; ++i)
	{
		DBConnection conn;

		sqlReturn = SQLAllocHandle(SQL_HANDLE_DBC, enviromentHandle, &conn.dbcHandle);
		if (sqlReturn != SQL_SUCCESS)
		{
			std::cout << "dbc handle alloc Failed : " << sqlReturn << std::endl;
			return false;
		}

		sqlReturn = SQLDriverConnect(conn.dbcHandle, NULL, (SQLWCHAR*)connectionString.c_str(), SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);
		if (LocalUtil::SQLIsSuccess(sqlReturn) == false)
		{
			std::cout << "ConnectSQLDriver() falied" << std::endl;
			return false;
		}

		sqlReturn = SQLAllocHandle(SQL_HANDLE_STMT, conn.dbcHandle, &conn.stmtHandle);
		if (sqlReturn != SQL_SUCCESS)
		{
			std::cout << "stmt handle alloc Failed : " << sqlReturn << std::endl;
			return false;
		}

		connectionList.push_back(conn);
	}

	SQLFreeHandle(SQL_HANDLE_ENV, enviromentHandle);
	return true;
}

bool AuthDBConnector::ConnectDB(const std::wstring& optionFileName)
{
	if (OptionParsing(optionFileName) == false)
	{
		std::cout << "OptionParsing() Failed" << std::endl;
		return false;
	}

	SQLHENV enviromentHandle;
	SQLRETURN sqlReturn;

	sqlReturn = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &enviromentHandle);
	if (sqlReturn != SQL_SUCCESS)
	{
		std::cout << "env handle alloc Failed : " << sqlReturn << std::endl;
		return false;
	}

	sqlReturn = SQLSetEnvAttr(enviromentHandle, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
	if (sqlReturn != SQL_SUCCESS)
	{
		std::cout << "SQLSetEnvAttr() Failed : " << sqlReturn << std::endl;
		return false;
	}

	DBConnection conn;

	sqlReturn = SQLAllocHandle(SQL_HANDLE_DBC, enviromentHandle, &conn.dbcHandle);
	if (sqlReturn != SQL_SUCCESS)
	{
		std::cout << "dbc handle alloc Failed : " << sqlReturn << std::endl;
		return false;
	}

	sqlReturn = SQLDriverConnect(conn.dbcHandle, NULL, (SQLWCHAR*)GetDBConnectionString().c_str(), SQL_NTS, NULL, 0, NULL, SQL_DRIVER_COMPLETE);
	if (LocalUtil::SQLIsSuccess(sqlReturn) == false)
	{
		std::cout << "ConnectSQLDriver() falied" << std::endl;
		return false;
	}

	sqlReturn = SQLAllocHandle(SQL_HANDLE_STMT, conn.dbcHandle, &conn.stmtHandle);
	if (sqlReturn != SQL_SUCCESS)
	{
		std::cout << "stmt handle alloc Failed : " << sqlReturn << std::endl;
		return false;
	}

	defaultConnection = conn;
	SQLFreeHandle(SQL_HANDLE_ENV, enviromentHandle);

	if (connectionPool.Initialize(GetDBConnectionString(), connectionPoolSize) == false)
	{
		std::cout << "Initialize connection pool failed" << std::endl;
		return false;
	}

	return true;

	return true;
}

bool AuthDBConnector::OptionParsing(const std::wstring& optionFileName)
{
	WCHAR buffer[BUFFER_MAX];
	LoadParsingText(buffer, optionFileName.c_str(), BUFFER_MAX);

	if (g_Paser.GetValue_String(buffer, L"ODBC", L"DSN", dsn) == false)
		return false;
	if (g_Paser.GetValue_String(buffer, L"ODBC", L"UID", uid) == false)
		return false;
	if (g_Paser.GetValue_String(buffer, L"ODBC", L"PWD", password) == false)
		return false;
	if (g_Paser.GetValue_String(buffer, L"ODBC", L"Schema", schemaName) == false)
		return false;
	if (g_Paser.GetValue_String(buffer, L"ODBC", L"CATALOG_NAME", catalogName) == false)
		return false;
	if (g_Paser.GetValue_String(buffer, L"ODBC", L"DRIVER", driver) == false)
		return false;
	if (g_Paser.GetValue_Int(buffer, L"CONNECTION_POOL", L"SIZE", &connectionPoolSize) == false)
		return false;

	return true;
}

const std::wstring AuthDBConnector::GetDBConnectionString()
{
	std::wstring connectString;
	connectString += L"DRIVER=";
	connectString += driver;
	connectString += L";SERVER=";
	connectString += dsn;
	connectString += L";DATABASE=";
	connectString += catalogName;
	connectString += L";UID=";
	connectString += uid;
	connectString += L";PWD=";
	connectString += password;

	return connectString;
}
