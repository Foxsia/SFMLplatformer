#include "Enemy.h"
#include "TileMap.h"
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

    Enemy::Enemy(int health, int lives, EnemyType type) : Entity(health, lives), type(type)
    {
        velocity = { 0.f, 0.f };
    }

    void Enemy::update(float dt, TileMap& map, GameContext& context)
    {
        if (!isAlive()) return;

        applyGravity(dt);
        move(dt);

        CollisionSystem::resolveEnemyTileCollision(*this, map);

        if (damageCooldown > 0.f) damageCooldown -= dt;
    }

    void Enemy::render(sf::RenderWindow& window)
    {
        if (!isAlive())
            return;

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
        velocity.y = vel;
    }

    void Enemy::setPosition(float x, float y)
    {
        sprite.setPosition(x, y);
    }
}