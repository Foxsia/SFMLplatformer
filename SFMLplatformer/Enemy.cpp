#include "Enemy.h"
#include "TileMap.h"
#include "Tile.h"
#include "CollisionSystem.h"

namespace fp
{
    Enemy::Enemy()
    {
        shape.setSize({ 32.f, 32.f });
        shape.setFillColor(sf::Color::Red);
        velocity = { 0.f, 0.f };
    }

    void Enemy::update(float dt, TileMap& map)
    {
        applyGravity(dt);
        move(dt);

        CollisionSystem::resolveEnemyTileCollision(
            *this, map
        );
        checkDirectionChange(map);
    }

    void Enemy::render(sf::RenderWindow& window)
    {
        window.draw(shape);
    }

    void Enemy::applyGravity(float dt)
    {
        velocity.y += gravity * dt;
    }

    void Enemy::move(float dt)
    {
        velocity.x = speed * direction;

        shape.move(velocity * dt);
    }

    void Enemy::checkDirectionChange(TileMap& map)
    {
        const sf::FloatRect bounds = shape.getGlobalBounds();

        const float footX = bounds.left + bounds.width / 2.f;
        const float footY = bounds.top + bounds.height + 5.f;

        const int tileX = static_cast<int>(footX) / map.getTileSize();
        const int tileY = static_cast<int>(footY) / map.getTileSize();

        Tile* tile = map.getTile(tileX, tileY);

        if (!tile)
        {
            direction *= -1;
        }
    }

    sf::FloatRect Enemy::getGlobalBounds() const
    {
        return shape.getGlobalBounds();
    }

    sf::Vector2f Enemy::getPosition() const
    {
        return shape.getPosition();
    }

    sf::RectangleShape& Enemy::getShape()
    {
        return shape;
    }

    void Enemy::setVelocityY(float vel)
    {
        this->velocity.y = vel;
    }

    void Enemy::setPosition(float x, float y)
    {
        shape.setPosition({ x, y });
    }
}