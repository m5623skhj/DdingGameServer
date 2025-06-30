#include "PreCompile.h"

#include <sql.h>
#include <sqlext.h>
#include "StoredProcedure.h"

namespace ODBCUtil
{
	static void PrintSQLErrorMessage(SQLHSTMT stmtHandle)
	{
		SQLWCHAR SqlState[6];
		SQLWCHAR Msg[SQL_MAX_MESSAGE_LENGTH];
		SQLINTEGER NativeError;
		SQLSMALLINT MsgLen;

		SQLRETURN rc = SQLGetDiagRec(SQL_HANDLE_STMT, stmtHandle, 1, SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen);
		const std::wstring errorMessage = Msg;
		if (SQL_SUCCEEDED(rc)) 
		{
			std::wcout << L"SQL error message : " << errorMessage << std::endl;
		}
	}

	static bool SQLIsSuccess(SQLRETURN returnValue)
	{
		if (returnValue == SQL_SUCCESS || returnValue == SQL_SUCCESS_WITH_INFO)
		{
			return true;
		}

		return false;
	}

	static bool IsSameType(const std::string& lhs, const std::string& rhs)
	{
		// string
		if (lhs == "FString")
		{
			if (rhs == "varchar")
			{
				return true;
			}
		}
		else if (rhs == "FString")
		{
			if (lhs == "varchar")
			{
				return true;
			}
		}

		// wstring
		if (lhs == "FWString")
		{
			if (rhs == "nvarchar")
			{
				return true;
			}
		}
		else if (rhs == "FWString")
		{
			if (lhs == "nvarchar")
			{
				return true;
			}
		}

		// int64
		if (lhs == "__int64")
		{
			if (rhs == "bigint")
			{
				return true;
			}
		}
		else if (rhs == "bigint")
		{
			if (lhs == "__int64")
			{
				return true;
			}
		}

		return false;

		return false;
	}

	static std::wstring GetDataTypeName(SQLSMALLINT inDataType)
	{
		switch (inDataType)
		{
		case SQL_BIGINT:
			return L"int64";
		case SQL_NUMERIC:
		case SQL_DECIMAL:
		case SQL_INTEGER:
			return L"int";
		case SQL_FLOAT:
		case SQL_REAL:
			return L"float";
		case SQL_WVARCHAR:
			return L"nvarchar";
		case SQL_VARCHAR:
			return L"string";
		case SQL_BIT:
			return L"bool";
		default:
			return L"";
		}
	}

	static bool DBSendQuery(const std::wstring& query, const SQLHSTMT& stmtHandle)
	{
		if (SQLExecute(stmtHandle) != SQL_SUCCESS)
		{
			ODBCUtil::PrintSQLErrorMessage(stmtHandle);
			return false;
		}

		return true;
	}

	static bool DBSendQueryDirect(const std::wstring& query, const SQLHSTMT& stmtHandle)
	{
		if (ODBCUtil::SQLIsSuccess(SQLExecDirect(stmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS)) == false)
		{
			ODBCUtil::PrintSQLErrorMessage(stmtHandle);
			return false;
		}

		return true;
	}

	static bool DBSendQueryWithPrepare(const std::wstring& query, const SQLHSTMT& stmtHandle)
	{
		if (SQLPrepare(stmtHandle, (SQLWCHAR*)query.c_str(), SQL_NTS) != SQL_SUCCESS)
		{
			ODBCUtil::PrintSQLErrorMessage(stmtHandle);
			return false;
		}

		if (SQLExecute(stmtHandle) != SQL_SUCCESS)
		{
			ODBCUtil::PrintSQLErrorMessage(stmtHandle);
			return false;
		}

		return true;
	}
}