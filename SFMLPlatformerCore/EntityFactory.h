#pragma once

#include <memory>
#include <unordered_map>

#include "IEntityCreator.h"

namespace fp
{
	class EntityFactory
	{
	public:
		static void registerCreator(int id, std::unique_ptr<IEntityCreator> creator);

		static bool create(int id, unsigned x, unsigned y, TileMap& tileMap, std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<Collectible>>& collectibles);

	private:
		static std::unordered_map<int, std::unique_ptr<IEntityCreator>> creators;
	};
}

