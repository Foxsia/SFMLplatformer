#include "Enemy.h"
#include "TileMap.h"
#include "CollisionSystem.h"
#include "ConfigManager.h"

namespace fp
{
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
        const auto& cfg = ConfigManager::get("enemy");
        auto hitBox = sprite.getGlobalBounds();

        hitBox.left += cfg["hitboxPaddingLeft"];
        hitBox.width -= cfg.at("hitboxPaddingLeft").get<float>() + cfg.at("hitboxPaddingRight").get<float>();

        hitBox.top += cfg["hitboxPaddingTop"];
        hitBox.height -= cfg.at("hitboxPaddingTop").get<float>() + cfg.at("hitboxPaddingBottom").get<float>();

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