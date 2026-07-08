#pragma once
#include <memory>
#include <vector>

namespace fp
{
	class TileMap;
	class Enemy;
	class Collectible;

	class IEntityCreator
	{
	public:
		virtual ~IEntityCreator() = default;

		virtual void create(unsigned x, unsigned y, TileMap& tileMap, std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<Collectible>>& collectibles) = 0;
	};
}