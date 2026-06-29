#include "SlimeEnemy.h"
#include "TileMap.h"
#include "Tile.h"
#include "CollisionSystem.h"

namespace fp
{
    namespace
    {
        const float ENEMY_HITBOX_PADDING_LEFT = 10.f;
        const float ENEMY_HITBOX_PADDING_RIGHT = 10.f;

        const float ENEMY_HITBOX_PADDING_TOP = 5.f;
        const float ENEMY_HITBOX_PADDING_BOTTOM = 0.f;
    }

    SlimeEnemy::SlimeEnemy() : Enemy(1, 1, EnemyType::Slime)
    {
        textureSheet.loadFromFile("assets/slime_green.png");

        sprite.setTexture(textureSheet);
        sprite.setTextureRect(sf::IntRect(0, 24, 24, 24));
        sprite.setScale(3.f, 3.f);

        velocity = { 0.f, 0.f };
    }

    void SlimeEnemy::update(float dt, TileMap& map, GameContext& context)
    {
        Enemy::update(dt, map, context);
        checkDirectionChange(map);

        if (damageCooldown > 0.f) damageCooldown -= dt;
    }

    void SlimeEnemy::render(sf::RenderWindow& window)
    {
        Enemy::render(window);
    }

    

    void SlimeEnemy::checkDirectionChange(TileMap& map)
    {
        const sf::FloatRect bounds = sprite.getGlobalBounds();

        const float footX = bounds.left + bounds.width / 2.f;
        const float footY = bounds.top + bounds.height + 5.f;

        const int tileX = static_cast<int>(footX) / map.getTileSize();
        const int tileY = static_cast<int>(footY) / map.getTileSize();

        Tile* tile = map.getTile(tileX, tileY);

        if (!tile)
        {
            direction *= -1;
        }

        if (direction > 0)
        {
            sprite.setScale(3.f, 3.f);
            sprite.setOrigin(0.f, 0.f);
        }
        else
        {
            sprite.setScale(-3.f, 3.f);
            sprite.setOrigin(sprite.getLocalBounds().width, 0.f);
        }
    }
}