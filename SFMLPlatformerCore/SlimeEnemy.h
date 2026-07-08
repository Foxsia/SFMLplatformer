#pragma once

#include <SFML/Graphics.hpp>
#include "AnimationComponent.h"
#include "Enemy.h"

namespace fp
{
    class TileMap;

    class SlimeEnemy : public Enemy
    {
    public:
        SlimeEnemy();

        void update(float dt, TileMap& map, GameContext& context) override;
        void render(sf::RenderWindow& window) override;

    private:
        void checkDirectionChange(TileMap& map);
    };
}
