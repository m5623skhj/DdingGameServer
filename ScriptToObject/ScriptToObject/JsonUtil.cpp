#include "JsonUtil.h"
#include <windows.h>
#include "DataUtil.h"
#include "Config.h"

#include "ZoneData.h"

#define ADD_TO_CONTAINER(className){ \
	std::cout << "Start loading : " << #className << ".json" << std::endl; \
	if(AddToDataContainerFromJsonFile<className>(currentFolder + Configuration::Config::GetInst().jsonScriptFolderPath + #className + ".json") == false) \
	{ \
		std::cout << "Load failed : " << #className << ".json" << std::endl; \
		return false; \
	} \
	std::cout << "Load completed : " << #className << ".json" << std::endl; \
}

#define ADD_TO_CONTAINER_WITH_FILE_NAME(className, fileName){ \
	std::cout << "Start loading : " << fileName << std::endl; \
	if(AddToDataContainerFromJsonFile<className>(currentFolder + Configuration::Config::GetInst().jsonScriptFolderPath + fileName) == false) \
	{ \
		std::cout << "Load failed : " << fileName << std::endl; \
		return false; \
	} \
	std::cout << "Load completed : " << fileName << std::endl; \
}

#define POST_LOAD(className){ \
	if(DataContainer<className::DataKeyType, className>::GetInst().PostLoad() == false) \
	{ \
		std::cout << "PostLoad() failed : " << #className << std::endl; \
	} \
}

namespace JsonUtil
{
	std::string GetCurrentFolder()
	{
		char buffer[MAX_PATH];
		GetModuleFileNameA(NULL, buffer, MAX_PATH);
		return std::string(buffer);
	}

	bool LoadAllDataScript()
	{
		if (Configuration::LoadConfiguration(Configuration::configurationFilePath) == false)
		{
			return false;
		}

		std::string currentFolder = GetCurrentFolder();

		std::cout << std::endl
			<< "--------------------------------------" << std::endl
			<< "Start load from " << currentFolder << std::endl
			<< "--------------------------------------" << std::endl;

#pragma region AddToContainer

		ADD_TO_CONTAINER(ZoneData);

#pragma endregion AddToContainer

		std::cout << std::endl
			<< "--------------------------------------" << std::endl
			<< "All files loaded successfully" << std::endl
			<< "Start PostLoad()"  << std::endl
			<< "--------------------------------------" << std::endl;

#pragma region PostLoad

		POST_LOAD(ZoneData);

#pragma endregion PostLoad

		std::cout << std::endl
			<< "--------------------------------------" << std::endl
			<< "PostLoad() successfully" << std::endl
			<< "--------------------------------------" << std::endl;

		return true;
	}
}