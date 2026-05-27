#pragma once

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
            std::vector<Enemy*>& enemies
        );
    };
}