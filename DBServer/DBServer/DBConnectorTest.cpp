#include "PreCompile.h"

#include "GoogleTest.h"
#include "ODBCConnector.h"
#include "StoredProcedure.h"
#include "ODBCMetaData.h"
#include "ODBCUtil.h"
#include <utility>
#include <vector>

#if UNIT_TEST
TEST(DBConnectorTest, DBInitializeTest)
{
	ODBCConnector& connector = ODBCConnector::GetInst();

	ASSERT_EQ(connector.ConnectDB(L"DBServerOptionFile/DBConnectFile.txt"), true);
	EXPECT_EQ(connector.InitDB(), true);

	connector.DisconnectDB();
}

TEST(DBConnectorTest, ProcedureParameterTest)
{
	std::map<std::string, std::shared_ptr<SP::IStoredProcedure>> testProcedureMap;
	std::map<std::string, std::vector<std::pair<std::string, ProcedureTypeName>>> resultPropertyMap;

	PROCEDURE_TEST_LIST(testProcedureMap, resultPropertyMap);

	ODBCConnector& connector = ODBCConnector::GetInst();

	ASSERT_EQ(connector.ConnectDB(L"DBServerOptionFile/DBConnectFile.txt"), true);
	EXPECT_EQ(connector.InitDB(), true);

	std::vector<ProcedureName> notMatchedProcedureList;
	auto columnMatch = [](
		const std::vector<std::pair<std::string, ProcedureTypeName>>& cppProperties
		, const std::vector<std::pair<std::string, ProcedureTypeName>>& dbProperties) -> bool
	{
		if (cppProperties.size() != dbProperties.size())
		{
			return false;
		}

		for (size_t i = 0; i < cppProperties.size(); ++i)
		{
			// �÷��� Ÿ�԰� ������ �˻�
			if (cppProperties[i].second != dbProperties[i].second)
			{
				if (ODBCUtil::IsSameType(cppProperties[i].second, dbProperties[i].second) == false)
				{
					return false;
				}
			}
		}

		return true;
	};

	for (const auto& testProcedure : testProcedureMap)
	{
		ASSERT_NE(testProcedure.second, nullptr);
	
		std::vector<std::pair<std::string, ProcedureTypeName>> cppProperties;
		testProcedure.second->GetTypeInfo().GetAllProperties(cppProperties);

		auto matchedProcedureInfo = connector.GetProcedureInfo(testProcedure.first);
		ASSERT_NE(matchedProcedureInfo, nullptr);
		auto matchedProcedureResultColumnInfo = resultPropertyMap.find(testProcedure.first);
		ASSERT_NE(matchedProcedureResultColumnInfo, resultPropertyMap.end());

		std::vector<std::pair<std::string, ProcedureTypeName>> dbInputProperties;
		char utf8Name[256], UTF8_dataTypeName[256];
		for (const auto& inputColumn : matchedProcedureInfo->inputColumnInfoList)
		{
			UTF16ToUTF8(inputColumn.name.c_str(), utf8Name);
			UTF16ToUTF8(inputColumn.dataTypeName.c_str(), UTF8_dataTypeName);
			dbInputProperties.emplace_back(utf8Name, UTF8_dataTypeName);
		}
		
		std::vector<std::pair<std::string, ProcedureTypeName>> dbResultProperties;
		for (const auto& resultColumn : matchedProcedureInfo->resultColumnInfoList)
		{
			UTF16ToUTF8(resultColumn.name.c_str(), utf8Name);
			UTF16ToUTF8(resultColumn.dataTypeName.c_str(), UTF8_dataTypeName);
			dbResultProperties.emplace_back(utf8Name, UTF8_dataTypeName);
		}

		bool isMatched = true;
		isMatched = columnMatch(cppProperties, dbInputProperties) && isMatched;
		isMatched = columnMatch(matchedProcedureResultColumnInfo->second, dbResultProperties) && isMatched;
		if (isMatched == false)
		{
			notMatchedProcedureList.emplace_back(testProcedure.first);
		}
		EXPECT_TRUE(isMatched);
	}

	if (!notMatchedProcedureList.empty())
	{
		std::cout << std::endl;
		std::cout << "---------------------------------------" << std::endl;
		for (const auto& notMatchedProcedure : notMatchedProcedureList)
		{
			std::cout << "Procedure was not matched. ProcedureName[" << notMatchedProcedure << "]" << std::endl;
		}
		std::cout << "---------------------------------------" << std::endl << std::endl << std::endl;
	}
	connector.DisconnectDB();
}
#endif