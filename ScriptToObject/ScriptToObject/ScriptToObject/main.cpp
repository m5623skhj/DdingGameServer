#include <iostream>
#include "TestData.h"
#include "DataUtil.h"
#include "JsonUtil.h"

using namespace JsonUtil;
using namespace std;

int main()
{
	if (LoadAllDataScript() == false)
	{
		cout << "Failed to load data" << std::endl;
		return 0;
	}

	auto temp1 = FIND_DATA(Test1, 2);
	auto tempErr = FIND_DATA(Test1, 100);

	auto temp2 = FIND_DATA(Test2, "test_2");

	return 0;
}
