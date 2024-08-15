#pragma once
#include "DataObjectBase.h"
#include "DataMacro.h"
#include "FromJsonFunction.h"

class DungeonData : public DataObjectBase
{
public:
	DungeonDataId id;
	std::string dungeonName;
	unsigned short dungeonLevel;

public:
	SET_DATA_OBJECT(DungeonData, id);

	virtual void LoadFromJson(const nlohmann::json& jsonObject) override
	{
		id = jsonObject["id"];
		dungeonName = jsonObject["dungeonName"];
		dungeonLevel = jsonObject["dungeonLevel"];
	}

	virtual bool PostLoad() override
	{
		return true;
	}
};