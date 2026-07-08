#include "FireFruitCreator.h"
#include "FireFruit.h"
#include "TileMap.h"

namespace fp
{
	void FireFruitCreator::create(unsigned x, unsigned y, TileMap& tileMap, std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<Collectible>>& collectibles)
	{
        auto fireFruit = std::make_unique<FireFruit>();

        fireFruit->setPosition(
            x * tileMap.getTileSize(),
            y * tileMap.getTileSize()
        );

        collectibles.push_back(std::move(fireFruit));
	}
}