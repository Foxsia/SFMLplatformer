#include "ConfigManager.h"
#include <fstream>

namespace fp
{
	void ConfigManager::load(const std::string& path)
	{
		std::ifstream in(path);
		in >> data;
	}
	const nlohmann::json& ConfigManager::get(const std::string& section)
	{
		return data.at(section);
	}
}