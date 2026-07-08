#include "FireEnemyCreator.h"
#include "TileMap.h"

namespace fp
{
    void FireEnemyCreator::create(unsigned x, unsigned y, TileMap& tileMap, std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<Collectible>>&)
    {
        auto enemy = std::make_unique<FireEnemy>();

        const float tileSize = static_cast<float>(tileMap.getTileSize());

        enemy->setPosition(
            x * tileSize,
            y * tileSize
        );

        enemies.push_back(std::move(enemy));
    }
}