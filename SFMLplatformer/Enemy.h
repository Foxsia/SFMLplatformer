#pragma once

#include "Entity.h"
#include <SFML/Graphics.hpp>
#include "GameContext.h"

namespace fp
{
    class TileMap;

    enum class EnemyType
    {
        Slime = 2,
        Fire = 8
    };

    namespace
    {
        const float GRAVITY = 900.f;
        const float SPEED = 60.f;
        const int DIRECTION = 1;
    }

    class Enemy : public Entity
    {
    public:
        Enemy(int health = 1, int lives = 1, EnemyType type = EnemyType::Slime);
        virtual ~Enemy() = default;

        virtual void update(float dt, TileMap& map, GameContext& context);
        virtual void render(sf::RenderWindow& window);

        virtual sf::FloatRect getGlobalBounds() const;
        virtual sf::FloatRect getHitbox() const;

        virtual sf::Vector2f getPosition() const;

        virtual void setPosition(float x, float y);
        virtual void setVelocityY(float vel);

        sf::Vector2f getVelocity() const
        {
            return velocity;
        }
        EnemyType getType() const { return type; }

    protected:
        void applyGravity(float dt);
        void move(float dt);

        sf::Sprite sprite;
        sf::Texture textureSheet;

        sf::Vector2f velocity;

        float gravity = GRAVITY;
        float speed = SPEED;

        int direction = DIRECTION;

        EnemyType type;
    };
}