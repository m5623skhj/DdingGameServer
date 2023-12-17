#include <iostream>
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

	return 0;
}
