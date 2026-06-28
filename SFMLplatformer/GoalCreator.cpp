#include "GoalCreator.h"
#include "Goal.h"
#include "TileMap.h"

namespace fp
{
	void GoalCreator::create(unsigned x, unsigned y, TileMap& tileMap, std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<Collectible>>& collectibles)
	{
        auto goal = std::make_unique<Goal>();

        goal->setPosition(
            x * tileMap.getTileSize(),
            y * tileMap.getTileSize()
        );

        collectibles.push_back(std::move(goal));
	}
}