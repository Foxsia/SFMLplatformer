#include "SpikeCreator.h"
#include "TileMap.h"

namespace fp
{
	void SpikeCreator::create(unsigned x, unsigned y, TileMap& tileMap, std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<Collectible>>& collectibles)
	{
		tileMap.addSpikeTile(x, y);
	}
}