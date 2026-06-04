#pragma once

#include "Player.h" 
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
            std::vector<Enemy*>& enemies
        );
    };
}