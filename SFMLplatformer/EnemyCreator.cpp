#include "EnemyCreator.h"
#include "SlimeEnemy.h"
#include "TileMap.h"

namespace fp
{
	void EnemyCreator::create(unsigned x, unsigned y, TileMap& tileMap, std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<Collectible>>&)
	{
		auto enemy = std::make_unique<SlimeEnemy>();

		enemy->setPosition(
			x * tileMap.getTileSize(),
			y * tileMap.getTileSize());

		enemies.push_back(std::move(enemy));
	}
}