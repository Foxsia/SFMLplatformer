#include "FireEnemy.h"
#include "TileMap.h"
#include "FireBall.h"
#include "GameContext.h"
#include "Player.h"
#include "Tile.h"
#include "ConfigManager.h"

namespace fp
{
    namespace
    {
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
        const auto& cfg = ConfigManager::get("fireEnemy");
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

        if (dy <= cfg.at("verticalSightDistance").get<float>())
        {
            if (!chasingPlayer)
            {
                if (std::abs(dx) <= cfg.at("followDistance").get<float>())
                    chasingPlayer = true;
            }
            else
            {
                if (std::abs(dx) >= cfg.at("loseDistance").get<float>())
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

        if (dx > cfg.at("stopDistance").get<float>())
            direction = 1;
        else if (dx < -cfg.at("stopDistance").get<float>())
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

            pos.x += (right ? cfg.at("fireballOffsetXRight").get<float>() : cfg.at("fireballOffsetXLeft").get<float>());
            pos.y += cfg.at("fireballOffsetY").get<float>();

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
        const auto& cfg = ConfigManager::get("fireEnemy");
        const sf::FloatRect bounds = sprite.getGlobalBounds();

        const float footX = direction > 0
            ? bounds.left + bounds.width + cfg.at("groundCheckOffsetX").get<float>()
            : bounds.left - cfg.at("groundCheckOffsetX").get<float>();

        const float footY = bounds.top + bounds.height + cfg.at("groundCheckOffsetY").get<float>();

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