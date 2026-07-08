#pragma once
#include <memory>
#include <vector>

namespace fp
{
	template <typename T>

	std::vector<T*> toRaw(const std::vector<std::unique_ptr<T>>& vec)
	{
		std::vector<T*> res;
		res.reserve(vec.size());

		for (auto& element : vec)
			res.push_back(element.get());
		return res;
	}

	inline std::string formatTime(float seconds)
	{
		int total = static_cast<int>(seconds);

		int minutes = total / 60;
		int secs = total % 60;

		return std::to_string(minutes) + ":" +
			(secs < 10 ? "0" : "") + std::to_string(secs);
	}

	inline std::string formatLevelName(const std::string& path)
	{
		std::string name = path;

		size_t pos = name.find("levels\\");
		if (pos != std::string::npos)
			name.erase(pos, 7);

		pos = name.find("levels/");
		if (pos != std::string::npos)
			name.erase(pos, 7);

		pos = name.find(".txt");
		if (pos != std::string::npos)
			name.erase(pos, 4);

		return name;
	}
}