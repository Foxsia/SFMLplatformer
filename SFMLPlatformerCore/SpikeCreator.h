#pragma once
#include "IEntityCreator.h"

namespace fp
{
	class SpikeCreator : public IEntityCreator
	{
	public:
		void create(unsigned x, unsigned y, TileMap& tileMap, std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<Collectible>>& collectibles) override;
	};
}

