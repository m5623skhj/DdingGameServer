#pragma once
#include "nlohmann/json.hpp"
#include "../../GameServer/GameServer/GameServerDefine.h"

void from_json(const nlohmann::json& jsonObject, Position& position);