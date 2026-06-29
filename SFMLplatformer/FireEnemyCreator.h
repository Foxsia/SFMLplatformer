#pragma once

#include "IEntityCreator.h"
#include "FireEnemy.h"

namespace fp
{
    class FireEnemyCreator : public IEntityCreator
    {
    public:
        void create(unsigned x, unsigned y, TileMap& tileMap, std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<Collectible>>&) override;
    };
}