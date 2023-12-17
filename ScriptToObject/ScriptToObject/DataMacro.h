#pragma once
#include "DataUtil.h"

#pragma region DataObjectGenerator

#define ADD_DATA_CLASS_TO_GENERATOR(className) \
AddDataClass(#className, []() -> DataObjectBase* {return new className;})

#define MAKE_DATA_OBJECT(className, typeName) \
DataObjectGenerator::GetInst().MakeDataObject<className>(typeName)

#pragma endregion DataObjectGenerator

#pragma region DataContainer

#define SET_DATA_OBJECT(classType, key) \
SET_DATA_KEY_TYPE(decltype(key)); \
SET_OBJECT_TYPE_NAME(classType); \
virtual void AddDataToDataContainer() override \
{ \
	ADD_DATA(classType, key, this); \
}

#define SET_DATA_KEY_TYPE(type) using DataKeyType = type
#define SET_OBJECT_TYPE_NAME(classType) \
virtual constexpr std::string_view GetObjectTypeName() override \
{ \
	return #classType; \
}

#define ADD_DATA(classType, key, value) \
DataContainer<classType::DataKeyType, classType>::GetInst().AddData(key, value)

#define FIND_DATA(classType, key) \
DataContainer<classType::DataKeyType, classType>::GetInst().FindData(key)

#pragma endregion DataContainer