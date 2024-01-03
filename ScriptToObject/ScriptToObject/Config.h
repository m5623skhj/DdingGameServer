#pragma once
#include <string>

namespace Configuration
{
	const std::string configurationFilePath = "ScriptToObjectOptionFile\\Config.json";

	struct Config
	{
	private:
		Config() = default;

	public:
		static Config& GetInst()
		{
			static Config instance;
			return instance;
		}

	public:
		std::string jsonScriptFolderPath;
	};

	bool LoadConfiguration(const std::string configurationFilePath)
	{
		std::ifstream configFile(configurationFilePath);
		if (configFile.is_open() == false)
		{
			std::cout << "Invalid configuration path : " << configurationFilePath << std::endl;
			return false;
		}

		try
		{
			nlohmann::json jsonData;
			configFile >> jsonData;

			Config::GetInst().jsonScriptFolderPath = jsonData["ScriptFolderPath"];
		}
		catch (nlohmann::json::parse_error& e)
		{
			std::cout << "Parsing error : " << e.what() << std::endl;
			return false;
		}

		return true;
	}
}
