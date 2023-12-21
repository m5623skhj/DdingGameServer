#pragma once
#include <string>
#include "nlohmann/json.hpp"

class DataObjectBase
{
public:
	virtual ~DataObjectBase()
	{
	}

	virtual void LoadFromJson(const nlohmann::json& jsonObject) = 0;
	virtual bool AddDataToDataContainer() = 0;

	virtual constexpr std::string_view GetObjectTypeName() = 0;
	virtual bool PostLoad() = 0;

protected:
	std::string objectTypeName = "";
};

template<typename T>
concept DataBased = std::is_base_of<DataObjectBase, T>::value;
