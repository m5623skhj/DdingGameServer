#include "DataUtil.h"
#include "DataRegister.h"

DataObjectGenerator::DataObjectGenerator()
{
	DATA_REGISTER();
}

DataObjectGenerator& DataObjectGenerator::GetInst()
{
	static DataObjectGenerator instance;
	return instance;
}

void DataObjectGenerator::AddDataClass(std::string_view dataClassName, DataObjectConstructor constructor)
{
	constructorMap.insert({ dataClassName, constructor });
}
