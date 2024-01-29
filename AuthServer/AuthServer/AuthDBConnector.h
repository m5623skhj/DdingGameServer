#pragma once
#include <sql.h>
#include <sqlext.h>
#include <optional>
#include <list>
#include <mutex>

struct DBConnection
{
	SQLHDBC dbcHandle;
	SQLHSTMT stmtHandle;

	bool SendQuery(const std::wstring& query);
};

class DBConnectionPool
{
public:
	DBConnectionPool() = default;
	~DBConnectionPool();

public:
	bool Initialize(const std::wstring& inConnectionString, int inPoolSize);
	void Cleanup();

public:
	std::optional<DBConnection> GetConnection();
	void FreeConnection(DBConnection& connection);

private:
	bool Initialize();

private:
	std::list<DBConnection> connectionList;
	std::mutex connectionLock;

private:
	int poolSize = 0;
	std::wstring connectionString;
};

class AuthDBConnector
{
private:
	AuthDBConnector() = default;
	~AuthDBConnector() = default;
	AuthDBConnector(const AuthDBConnector& other) = delete;
	AuthDBConnector& operator=(const AuthDBConnector& other) = delete;

public:
	static AuthDBConnector& GetInst()
	{
		static AuthDBConnector instance;
		return instance;
	}

public:
	bool ConnectDB(const std::wstring& optionFileName);

private:
	bool OptionParsing(const std::wstring& optionFileName);
	const std::wstring GetDBConnectionString();

private:
	DBConnectionPool connectionPool;
	DBConnection defaultConnection;

private:
	WCHAR dsn[16];
	WCHAR uid[16];
	WCHAR password[16];
	WCHAR catalogName[16];
	WCHAR schemaName[16];
	WCHAR driver[64];

	int connectionPoolSize = 0;
};