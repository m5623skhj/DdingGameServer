#pragma once
#include "DataObjectBase.h"
#include "DataMacro.h"
#include "FromJsonFunction.h"

class NPCData : public DataObjectBase
{
public:
	NPCDataId id;
	std::string npcName;
	unsigned int npcLevel;

public:
	SET_DATA_OBJECT(NPCData, id);

	virtual void LoadFromJson(const nlohmann::json& jsonObject) override
	{
		id = jsonObject["id"];
		npcName = jsonObject["npcName"];
		npcLevel = jsonObject["npcLevel"];
	}

	virtual bool PostLoad() override
	{
		return true;
	}
};