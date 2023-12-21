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

bool DataObjectGenerator::AddDataClass(std::string_view dataClassName, DataObjectConstructor constructor)
{
	return constructorMap.insert({ dataClassName, constructor }).second;
}
