#pragma once

#include "ZoneData.h"

#pragma region DataRegister

#define DATA_REGISTER(){ \
	ADD_DATA_CLASS_TO_GENERATOR(ZoneData); \
}

#pragma endregion DataRegister