#pragma once
#include <string>
#include <vector>

namespace fp
{
	struct ScoreEntry
	{
		std::string name;
		int score;
		std::string time;
		std::string level;
	};

	class ScoreManager
	{
	public:
		static void addScore(const ScoreEntry & entry);
		static std::vector<ScoreEntry> loadScores();
	};
}

