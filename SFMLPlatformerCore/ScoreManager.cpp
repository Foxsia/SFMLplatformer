#include "ScoreManager.h"
#include <fstream>
#include <filesystem>
#include<iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace fp
{
	void ScoreManager::addScore(const ScoreEntry& entry)
	{
		std::filesystem::create_directories("data");

		json j = json::array();

		std::ifstream in("data/scores.json");
		if (in.is_open())
		{
			try
			{
				in >> j;
				if (!j.is_array())
					j = json::array();
			}
			catch (...)
			{
				j = json::array();
			}
		}

		json obj = {
			{"name", entry.name},
			{"score", entry.score},
			{"time", entry.time},
			{"level", entry.level}
		};

		j.push_back(obj);

		std::ofstream out("data/scores.json");
		if (!out.is_open())
		{
			std::cout << "FAILED TO WRITE FILE" << std::endl;
			return;
		}
		out << j.dump(4);
	}
	std::vector<ScoreEntry> ScoreManager::loadScores()
	{
		std::vector<ScoreEntry> result;

		std::ifstream in("data/scores.json");
		if (!in.is_open()) return result;

		json j;

		in >> j;

		for (auto& item : j)
		{
			ScoreEntry entry;
			entry.name = item["name"];
			entry.score = item["score"];
			entry.time = item["time"];
			entry.level = item["level"];

			result.push_back(entry);
		}

		return result;
	}
}