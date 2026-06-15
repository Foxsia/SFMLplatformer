#include "Enemy.h"
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

    Enemy::Enemy() : Entity(1, 1)
    {
        textureSheet.loadFromFile("assets/slime_green.png");

        sprite.setTexture(textureSheet);
        sprite.setTextureRect(sf::IntRect(0, 24, 24, 24));
        sprite.setScale(3.f, 3.f);

        velocity = { 0.f, 0.f };
    }

    void Enemy::update(float dt, TileMap& map)
    {
        if (!isAlive()) return;
        applyGravity(dt);
        move(dt);

        CollisionSystem::resolveEnemyTileCollision(
            *this, map
        );
        checkDirectionChange(map);

        if (damageCooldown > 0.f) damageCooldown -= dt;
    }

    void Enemy::render(sf::RenderWindow& window)
    {
        if (!isAlive()) return;
        window.draw(sprite);
    }

    void Enemy::applyGravity(float dt)
    {
        velocity.y += gravity * dt;
    }

    void Enemy::move(float dt)
    {
        velocity.x = speed * direction;

        sprite.move(velocity * dt);
    }

    void Enemy::checkDirectionChange(TileMap& map)
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

    sf::FloatRect Enemy::getGlobalBounds() const
    {
        return sprite.getGlobalBounds();
    }

    sf::FloatRect Enemy::getHitbox() const
    {
        auto hitBox = sprite.getGlobalBounds();

        hitBox.left += ENEMY_HITBOX_PADDING_LEFT;
        hitBox.width -= ENEMY_HITBOX_PADDING_LEFT + ENEMY_HITBOX_PADDING_RIGHT;

        hitBox.top += ENEMY_HITBOX_PADDING_TOP;
        hitBox.height -= ENEMY_HITBOX_PADDING_TOP + ENEMY_HITBOX_PADDING_BOTTOM;

        return hitBox;
    }

    sf::Vector2f Enemy::getPosition() const
    {
        return sprite.getPosition();
    }

    void Enemy::setVelocityY(float vel)
    {
        this->velocity.y = vel;
    }

    void Enemy::setPosition(float x, float y)
    {
        sprite.setPosition({ x, y });
    }
}