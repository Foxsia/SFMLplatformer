#include "FireEnemy.h"
#include "TileMap.h"
#include "FireBall.h"
#include "GameContext.h"
#include "Player.h"
#include "Tile.h"

namespace fp
{
    namespace
    {
        constexpr float FollowDistance = 300.f;
        constexpr float LoseDistance = 360.f;
        constexpr float StopDistance = 20.f;
        constexpr float VerticalSightDistance = 200.f;

        constexpr float FireballOffsetXRight = 40.f;
        constexpr float FireballOffsetXLeft = -10.f;
        constexpr float FireballOffsetY = 20.f;

        constexpr float GroundCheckOffsetX = 1.f;
        constexpr float GroundCheckOffsetY = 5.f;

        constexpr float SpriteScale = 3.f;
    }

    FireEnemy::FireEnemy() : Enemy(1, 1, EnemyType::Fire)
    {
        textureSheet.loadFromFile("assets/slime_purple.png");

        sprite.setTexture(textureSheet);
        sprite.setTextureRect(sf::IntRect(0, 24, 24, 24));
        sprite.setScale(3.f, 3.f);

        chasingPlayer = false;
        velocity = { 0.f, 0.f };
    }

    void FireEnemy::update(float dt, TileMap& map, GameContext& context)
    {
        if (!isAlive()) return;

        shootTimer += dt;

        sf::Vector2f enemyPos = sprite.getPosition();
        sf::Vector2f playerPos = context.player->getPosition();

        sf::Vector2f diff = playerPos - enemyPos;

        float dx = diff.x;
        float dy = std::abs(diff.y);

        if (chasingPlayer)
        {
            facing = (dx >= 0) ? 1 : -1;
        }

        if (dy <= VerticalSightDistance)
        {
            if (!chasingPlayer)
            {
                if (std::abs(dx) <= FollowDistance)
                    chasingPlayer = true;
            }
            else
            {
                if (std::abs(dx) >= LoseDistance)
                    chasingPlayer = false;
            }
        }
        else
        {
            chasingPlayer = false;
        }

        if (!chasingPlayer)
        {
            checkDirectionChange(map);
            Enemy::update(dt, map, context);
            return;
        }

        if (dx > StopDistance)
            direction = 1;
        else if (dx < -StopDistance)
            direction = -1;
        else
            direction = 0;

        if (direction > 0)
        {
            sprite.setScale(SpriteScale, SpriteScale);
            sprite.setOrigin(0.f, 0.f);
        }
        else if (direction < 0)
        {
            sprite.setScale(-SpriteScale, SpriteScale);
            sprite.setOrigin(sprite.getLocalBounds().width, 0.f);
        }

        checkDirectionChange(map);

        Enemy::update(dt, map, context);

        bool right = (facing == 1);

        if (shootTimer >= shootCooldown)
        {
            shootTimer = 0.f;

            sf::Vector2f pos = sprite.getPosition();

            pos.x += (right ? FireballOffsetXRight : FireballOffsetXLeft);
            pos.y += FireballOffsetY;

            context.fireballs->push_back(
                std::make_unique<FireBall>(pos, right, Team::Enemy)
            );
        }
    }
    void FireEnemy::render(sf::RenderWindow& window)
    {
        Enemy::render(window);
    }
    void FireEnemy::checkDirectionChange(TileMap& map)
    {
        const sf::FloatRect bounds = sprite.getGlobalBounds();

        const float footX = direction > 0
            ? bounds.left + bounds.width + GroundCheckOffsetX
            : bounds.left - GroundCheckOffsetX;

        const float footY = bounds.top + bounds.height + GroundCheckOffsetY;

        const int tileX = static_cast<int>(footX) / map.getTileSize();
        const int tileY = static_cast<int>(footY) / map.getTileSize();

        Tile* tile = map.getTile(tileX, tileY);

        if (!tile)
        {
            direction = 0;
        }

        if (direction > 0)
        {
            sprite.setScale(SpriteScale, SpriteScale);
            sprite.setOrigin(0.f, 0.f);
        }
        else
        {
            sprite.setScale(-SpriteScale, SpriteScale);
            sprite.setOrigin(sprite.getLocalBounds().width, 0.f);
        }
    }
}