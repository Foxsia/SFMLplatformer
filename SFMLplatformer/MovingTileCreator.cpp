#include "MovingTileCreator.h"
#include "TileMap.h"

namespace fp
{
	void MovingTileCreator::create(unsigned x, unsigned y, TileMap& tileMap, std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<Collectible>>& collectibles)
	{
		tileMap.addMovingTile(x, y);
	}
}
