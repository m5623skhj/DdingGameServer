#pragma once
#include <map>
#include <unordered_map>
#include <functional>
#include <concepts>
#include "DataObjectBase.h"

using DataObjectConstructor = std::function<DataObjectBase*()>;

#pragma region DataObjectGenerator

class DataObjectGenerator
{
private:
	DataObjectGenerator();

public:
	static DataObjectGenerator& GetInst();

	void AddDataClass(std::string_view dataClassName, DataObjectConstructor constructor);

	template <DataBased T>
	T* MakeDataObject(std::string_view dataClassName)
	{
		const auto& iter = constructorMap.find(dataClassName);
		if (iter == constructorMap.end())
		{
			return nullptr;
		}

		return static_cast<T*>(iter->second());
	}

private:
	std::map<std::string_view, DataObjectConstructor> constructorMap;
};

#pragma endregion DataObjectGenerator

#pragma region DataContainer

template<typename DataKeyType, DataBased DataValueType>
class DataContainer
{
private:
	DataContainer() = default;

public:
	static DataContainer& GetInst()
	{
		static DataContainer instance;
		return instance;
	}

	const DataValueType* FindData(DataKeyType key)
	{
		const auto& data = dataMap.find(key);
		if (data == dataMap.end())
		{
			return nullptr;
		}

		return data->second;
	}

	void AddData(DataKeyType key, DataValueType* value)
	{
		dataMap.insert({ key, value });
	}

	bool PostLoad()
	{
		for (auto& data : dataMap)
		{
			if (data.second->PostLoad() == false)
			{
				return false;
			}
		}

		return true;
	}

private:
	std::unordered_map<DataKeyType, DataValueType*> dataMap;
};

#pragma endregion DataContainer