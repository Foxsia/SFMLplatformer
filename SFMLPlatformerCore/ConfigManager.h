#pragma once
#include <nlohmann/json.hpp>

namespace fp
{
	class ConfigManager
	{
	public:
		static void load(const std::string& path);

		static const nlohmann::json& get(const std::string& section);

	private:
		static inline nlohmann::json data;
	};
}
