#pragma once

#include "Player.h"
#include "Collectible.h"
#include <string>
#include <vector>

namespace fp
{
    class TileMap;
    class Enemy;

    class LevelLoader
    {
    public:
        static void load(
            const std::string& filename,
            TileMap& tileMap,
            Player& player,
            std::vector<std::unique_ptr<Enemy>>& enemies,
            std::vector<std::unique_ptr<Collectible>>& collectibles
        );
    };
}