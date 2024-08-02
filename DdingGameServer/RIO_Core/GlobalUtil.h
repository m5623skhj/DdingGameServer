#pragma once
#include "EnumType.h"

bool IsSuccess(const ERROR_CODE& errorCode);

template<typename FromType, typename ToType>
requires std::is_integral_v<FromType>
and std::is_integral_v<ToType>
bool GuardIntegralTypeCasting(FromType value, OUT ToType& result)
{
	result = static_cast<ToType>(value);
	return result == value;
}