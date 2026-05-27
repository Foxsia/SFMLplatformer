#pragma once

#include <SFML/Graphics.hpp>

namespace fp
{
    class TileMap;

    class Enemy
    {
    public:
        Enemy();

        void update(float dt, TileMap& map);
        void render(sf::RenderWindow& window);

        sf::FloatRect getGlobalBounds() const;

        sf::Vector2f getPosition() const;
        sf::RectangleShape& getShape();
        sf::Vector2f getVelocity() const { return this->velocity; };
        void setVelocityY(float vel);
        void setPosition(float x, float y);

    private:
        void applyGravity(float dt);
        void move(float dt);
        void checkDirectionChange(TileMap& map);

    private:
        sf::RectangleShape shape;

        sf::Vector2f velocity;

        float speed = 60.f;
        float gravity = 900.f;

        int direction = -1;
    };
}