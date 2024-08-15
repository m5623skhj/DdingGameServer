#pragma once
#include "DataObjectBase.h"
#include "DataMacro.h"
#include "FromJsonFunction.h"

class ZoneData : public DataObjectBase
{
public:
	ZoneDataId id;
	std::string zoneName;
	Position minPosition;
	Position maxPosition;

public:
	SET_DATA_OBJECT(ZoneData, id);

	virtual void LoadFromJson(const nlohmann::json& jsonObject) override
	{
		id = jsonObject["id"];
		zoneName = jsonObject["zoneName"];
		minPosition = jsonObject["minPosition"];
		maxPosition = jsonObject["maxPosition"];
	}

	virtual bool PostLoad() override
	{
		return true;
	}
};