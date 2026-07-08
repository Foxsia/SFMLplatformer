#include "LifeFruitCreator.h"
#include "LifeFruit.h"
#include "TileMap.h"

namespace fp
{
	void LifeFruitCreator::create(unsigned x, unsigned y, TileMap& tileMap, std::vector<std::unique_ptr<Enemy>>&, std::vector<std::unique_ptr<Collectible>>& collectibles)
	{
		auto fruit = std::make_unique<LifeFruit>();

		fruit->setPosition(
			x * tileMap.getTileSize(),
			y * tileMap.getTileSize());

		collectibles.push_back(std::move(fruit));
	}
}