#include "LevelLoader.h"

#include "TileMap.h"
#include "Enemy.h"

#include <fstream>

namespace fp
{
    void LevelLoader::load(const std::string& filename, TileMap& tileMap, Player& player, std::vector<Enemy*>& enemies)
    {
        std::ifstream file(filename);

        if (!file.is_open()) return;

        float spawnX;
        float spawnY;

        file >> spawnX >> spawnY;

        tileMap.setPlayerSpawn(spawnX, spawnY);

        float tileSize = static_cast<float>(tileMap.getTileSize());

        float playerWidth = player.getGlobalBounds().width;
        float playerHeight = player.getGlobalBounds().height;

        player.setPosition(spawnX + (tileSize - playerWidth) / 2.f, spawnY - playerHeight);

        player.resetVelocityX();
        player.resetVelocityY();

        for (auto enemy : enemies)
        {
            delete enemy;
        }

        enemies.clear();

        for (unsigned x = 0; x < tileMap.getWidth(); x++)
        {
            for (unsigned y = 0; y < tileMap.getHeight(); y++)
            {
                tileMap.removeTile(x, y);
            }
        }

        for (unsigned y = 0; y < tileMap.getHeight(); y++)
        {
            for (unsigned x = 0; x < tileMap.getWidth(); x++)
            {
                int value;

                file >> value;

                if (value == 1)
                {
                    tileMap.addTile(x, y);
                }

                if (value == 2)
                {
                    Enemy* enemy = new Enemy();

                    enemy->setPosition(
                        x * tileMap.getTileSize(),
                        y * tileMap.getTileSize()
                    );

                    enemies.push_back(enemy);
                }
            }
        }
    }
}