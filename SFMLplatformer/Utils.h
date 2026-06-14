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
}