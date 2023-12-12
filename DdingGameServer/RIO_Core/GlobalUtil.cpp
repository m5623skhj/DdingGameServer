#include "PreCompile.h"
#include "GlobalUtil.h"

bool IsSuccess(const ERROR_CODE& errorCode)
{
	if (errorCode == ERROR_CODE::SUCCESS)
	{
		return true;
	}

	return false;
}