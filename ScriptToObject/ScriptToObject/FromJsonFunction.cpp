#include "PreCompile.h"
#include "FromJsonFunction.h"

void from_json(const nlohmann::json& jsonObject, Position& position)
{
	position.x = jsonObject.at("x").get<UINT>();
	position.y = jsonObject.at("y").get<UINT>();
}
