#include "PreCompile.h"
#include "ODBCMetaData.h"
#include "ODBCConnector.h"
#include "ODBCConst.h"

#include <sql.h>
#include <sqlext.h>
#include <cstring>

ColumnInfo::ColumnInfo(SQLTCHAR* inName, SQLSMALLINT inType,
	SQLINTEGER inDataType, SQLTCHAR* inDataTypeName, SQLULEN inColumnSize)
	: name(reinterpret_cast<WCHAR*>(inName))
	, columnType(inType)
	, dataType(inDataType)
	, dataTypeName(reinterpret_cast<WCHAR*>(inDataTypeName))
	, columnSize(inColumnSize)
{
}

ColumnInfo::ColumnInfo(SQLTCHAR* inName, SQLSMALLINT inType,
	SQLINTEGER inDataType, std::wstring inDataTypeName, SQLULEN inColumnSize)
	: name(reinterpret_cast<WCHAR*>(inName))
	, columnType(inType)
	, dataType(inDataType)
	, dataTypeName(inDataTypeName)
	, columnSize(inColumnSize)
{
}

bool ProcedureInfo::SettingDefaultSPMaker(SQLHSTMT stmtHandle)
{
	int cnt = 1;
	for (const auto& inputColumn : inputColumnInfoList)
	{
		auto defaultColumn = GetDefaultValue(inputColumn.dataType);
		if (defaultColumn == nullptr)
		{
			return false;
		}

		if (ODBCUtil::SQLIsSuccess(SQLBindParameter(stmtHandle, cnt, SQL_PARAM_INPUT, SQL_C_LONG, inputColumn.dataType, 0, 0, (SQLPOINTER)(defaultColumn.get()), 0, NULL)) == false)
		{
			ODBCUtil::PrintSQLErrorMessage(stmtHandle);
			return false;
		}

		++cnt;
	}

	return true;
}

std::shared_ptr<void> ProcedureInfo::GetDefaultValue(short dataType)
{
	if (dataType == SQL_NUMERIC || dataType == SQL_DECIMAL || dataType == SQL_INTEGER)
	{
		return std::make_shared<int>(0);
	}
	else if (dataType == SQL_BIGINT)
	{
		return std::make_shared<INT64>(0);
	}
	else if (dataType == SQL_FLOAT || dataType == SQL_REAL)
	{
		return std::make_shared<float>(0.f);
	}
	else if (dataType == SQL_WVARCHAR)
	{
		return std::make_shared<const WCHAR*>(L" ");
	}
	else if (dataType == SQL_VARCHAR)
	{
		return std::make_shared<const char*>(" ");
	}
	else if (dataType == SQL_BIT)
	{
		return std::make_shared<bool>(false);
	}

	return nullptr;
}

ODBCMetaData::ODBCMetaData(const std::wstring& inCatalogName)
	: catalogName(inCatalogName)
{
}

bool ODBCMetaData::GetProcedureNameFromDB(ODBCConnector& connector, WCHAR* catalogName, WCHAR* schemaName, OUT std::set<ProcedureName>& procedureNameList)
{
	auto stmtHandle = connector.GetDefaultStmtHandle();
	if (stmtHandle == nullptr)
	{
		return false;
	}

	if (SQLProcedures(stmtHandle, (SQLWCHAR*)catalogName, static_cast<SQLSMALLINT>(wcslen(catalogName)), schemaName, wcslen(schemaName), NULL, NULL) != SQL_SUCCESS)
	{
		return false;
	}

	SQLCHAR procedureName[256];
	ZeroMemory(procedureName, sizeof(procedureName));

	SQLRETURN ret;
	while (SQLFetch(stmtHandle) == SQL_SUCCESS)
	{
		ret = SQLGetData(stmtHandle, COLUMN_NUMBER::PROCEDURE_NAME, SQL_C_CHAR, procedureName, sizeof(procedureName), nullptr);
		if (ODBCUtil::SQLIsSuccess(ret) == false)
		{
			return false;
		}

		std::string procInfo = (const char*)procedureName;
		auto pos = procInfo.find(';');
		std::string procName = procInfo.substr(0, pos);

		procedureNameList.insert(procName.c_str());
	}

	SQLCloseCursor(stmtHandle);

	return true;
}

bool ODBCMetaData::MakeProcedureColumnInfoFromDB(ODBCConnector& connector, const std::set<ProcedureName>& procedureNameList)
{
	auto stmtHandle = connector.GetDefaultStmtHandle();
	// auto commit mode off
	// SQLExecute()�� ��� �ѹ��ϱ� ���ؼ� ����
	auto ret = SQLSetConnectAttr(connector.GetDefaultDBCHandle(), SQL_ATTR_AUTOCOMMIT, SQL_AUTOCOMMIT_OFF, 0);
	if (ODBCUtil::SQLIsSuccess(ret) == false)
	{
		std::cout << "SQLSetConnectAttr failed : " << std::endl;
		ODBCUtil::PrintSQLErrorMessage(stmtHandle);
		return false;
	}

	for (const auto& procedureName : procedureNameList)
	{
		auto procedureInfo = std::make_shared<ProcedureInfo>();
		WCHAR procedureNameBuffer[64];
		ZeroMemory(procedureNameBuffer, sizeof(procedureNameBuffer));

		if (UTF8ToUTF16(procedureName.c_str(), procedureNameBuffer, sizeof(procedureNameBuffer)) == false)
		{
			return false;
		}

		if (MakeInputColumnToProcedureInfo(stmtHandle, procedureName, procedureNameBuffer, procedureInfo) == false)
		{
			return false;
		}

		ret = SQLPrepare(stmtHandle, (SQLWCHAR*)procedureInfo->sql.c_str(), SQL_NTS);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
		{
			std::cout << "SQLPrepare failed : " << procedureName << std::endl;
			ODBCUtil::PrintSQLErrorMessage(stmtHandle);
			return false;
		}
		procedureInfo->SettingDefaultSPMaker(stmtHandle);

		// �ݵ�� ���� SQLExecute() Ȥ�� SQLExecDirect()�� ���� ȣ���ؾ� ��
		// ȣ������ ������, SQLNumResultCols()�� ȣ���� ��, coulmnCount�� 0�� ��ȯ��
		ret = SQLExecute(stmtHandle);
		if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO)
		{
			std::cout << "SQLExecute failed : " << procedureName << std::endl;
			ODBCUtil::PrintSQLErrorMessage(stmtHandle);
			return false;
		}

		if (MakeOutputColumnToProcedureInfo(stmtHandle, procedureName, procedureInfo) == false)
		{
			return false;
		}

		// ������ ȣ��� sp�� ����Ǹ� �ȵǹǷ� �ѹ��Ŵ
		SQLEndTran(SQL_HANDLE_DBC, connector.GetDefaultDBCHandle(), SQL_ROLLBACK);
		
		procedureInfoMap.insert({ procedureName, procedureInfo });
	}

	// auto commit mode on
	ret = SQLSetConnectAttr(connector.GetDefaultDBCHandle(), SQL_ATTR_AUTOCOMMIT, (SQLPOINTER)SQL_AUTOCOMMIT_ON, 0);
	if (ODBCUtil::SQLIsSuccess(ret) == false)
	{
		std::cout << "SQLSetStmtAttr() failed" << std::endl;
		ODBCUtil::PrintSQLErrorMessage(stmtHandle);
		return false;
	}

	return true;
}

bool ODBCMetaData::MakeInputColumnToProcedureInfo(SQLHSTMT stmtHandle, const ProcedureName& procedureName, const WCHAR* procedureNameBuffer, OUT std::shared_ptr<ProcedureInfo> outProcdureInfo)
{
	auto ret = SQLProcedureColumns(stmtHandle, NULL, 0, NULL, 0, (SQLWCHAR*)procedureNameBuffer, SQL_NTS, NULL, 0);
	if (ODBCUtil::SQLIsSuccess(ret) == false)
	{
		return false;
	}

	struct InputColumnInfo
	{
		SQLTCHAR name[64];
		SQLSMALLINT columnType = 0;
		SQLINTEGER dataType = 0;
		SQLTCHAR dataTypeName[64];
		SQLULEN columnSize = 0;

		bool BindColumn(SQLHSTMT stmtHandle)
		{
			if (SQLBindCol(stmtHandle, COLUMN_NUMBER::COLUMN_NAME, SQL_C_TCHAR, name, sizeof(name), nullptr) != SQL_SUCCESS)
			{
				return false;
			}
			if (SQLBindCol(stmtHandle, COLUMN_NUMBER::COLUMN_TYPE, SQL_C_SHORT, &columnType, sizeof(columnType), nullptr) != SQL_SUCCESS)
			{
				return false;
			}
			if (SQLBindCol(stmtHandle, COLUMN_NUMBER::DATA_TYPE, SQL_INTEGER, &dataType, sizeof(dataType), nullptr) != SQL_SUCCESS)
			{
				return false;
			}
			if (SQLBindCol(stmtHandle, COLUMN_NUMBER::DATA_TYPE_NAME, SQL_C_TCHAR, dataTypeName, sizeof(dataTypeName), nullptr) != SQL_SUCCESS)
			{
				return false;
			}
			if (SQLBindCol(stmtHandle, COLUMN_NUMBER::COLUMN_SIZE, SQL_C_LONG, &columnSize, sizeof(columnSize), nullptr) != SQL_SUCCESS)
			{
				return false;
			}

			return true;
		}
	};

	InputColumnInfo columnInfo;
	ZeroMemory(&columnInfo, sizeof(columnInfo));
	if (columnInfo.BindColumn(stmtHandle) == false)
	{
		return false;
	}

	outProcdureInfo->sql = L"{CALL ";
	outProcdureInfo->sql += procedureNameBuffer;
	outProcdureInfo->sql += L"(";

	bool isFirstParam = true;
	while (true)
	{
		if (ODBCUtil::SQLIsSuccess(SQLFetch(stmtHandle)) == false)
		{
			ODBCUtil::PrintSQLErrorMessage(stmtHandle);
			break;
		}
		
		if (columnInfo.columnType == SQL_PARAM_INPUT)
		{
			if (isFirstParam)
			{
				isFirstParam = false;
			}
			else
			{
				outProcdureInfo->sql += L", ";
			}

			outProcdureInfo->inputColumnInfoList.emplace_back(ColumnInfo(
				columnInfo.name, columnInfo.columnType, columnInfo.dataType, columnInfo.dataTypeName, columnInfo.columnSize));

			outProcdureInfo->sql += L"?";
		}
	}
	outProcdureInfo->sql += L")}";

	SQLCloseCursor(stmtHandle);

	return true;
}

bool ODBCMetaData::MakeOutputColumnToProcedureInfo(SQLHSTMT stmtHandle, const ProcedureName& procedureName, OUT std::shared_ptr<ProcedureInfo> procdureInfo)
{
	SQLSMALLINT columnCount = 0;
	SQLRETURN ret = SQLNumResultCols(stmtHandle, &columnCount);
	if (ODBCUtil::SQLIsSuccess(ret) == false)
	{
		std::cout << "SQLNumResultCols failed : " << procedureName << std::endl;
		ODBCUtil::PrintSQLErrorMessage(stmtHandle);
		return false;
	}

	struct ResultColumnInfo
	{
		SQLWCHAR name[64];
		SQLSMALLINT nameLength = 0;
		SQLSMALLINT dataType = 0;
		SQLULEN columnSize = 0;
		SQLSMALLINT decimalDigits = 0;
		SQLSMALLINT nullable = 0;
	};

	for (int i = 1; i <= columnCount; ++i)
	{
		ResultColumnInfo resultColumn;
		if (ODBCUtil::SQLIsSuccess(SQLDescribeCol(stmtHandle, static_cast<SQLUSMALLINT>(i)
			, resultColumn.name
			, _countof(resultColumn.name)
			, &resultColumn.nameLength
			, &resultColumn.dataType
			, &resultColumn.columnSize
			, &resultColumn.decimalDigits
			, &resultColumn.nullable)) == false)
		{
			std::cout << "SQLDescribeCol failed : " << procedureName << ", " << i << std::endl;
			ODBCUtil::PrintSQLErrorMessage(stmtHandle);
			return false;
		}

		procdureInfo->resultColumnInfoList.emplace_back(ColumnInfo(
			resultColumn.name, resultColumn.dataType, resultColumn.dataType, ODBCUtil::GetDataTypeName(resultColumn.dataType), resultColumn.columnSize));
	}
	SQLCloseCursor(stmtHandle);

	return true;
}

const ProcedureInfo* const ODBCMetaData::GetProcedureInfo(ProcedureName& procedureName) const
{
	auto it = procedureInfoMap.find(procedureName);
	if (it == procedureInfoMap.end())
	{
		return nullptr;
	}

	return it->second.get();
}