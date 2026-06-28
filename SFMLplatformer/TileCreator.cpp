#include "TileCreator.h"
#include "TileMap.h"

namespace fp
{
	void TileCreator::create(unsigned x, unsigned y, TileMap& tileMap, std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<Collectible>>& collectibles)
	{
		tileMap.addTile(x, y);
	}
}