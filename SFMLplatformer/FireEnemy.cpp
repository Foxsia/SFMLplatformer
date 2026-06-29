#include "FireEnemy.h"
#include "TileMap.h"
#include "FireBall.h"
#include "GameContext.h"
#include "Player.h"

namespace fp
{
    FireEnemy::FireEnemy() : Enemy(1, 1, EnemyType::Fire)
    {
        textureSheet.loadFromFile("assets/slime_purple.png");

        sprite.setTexture(textureSheet);
        sprite.setTextureRect(sf::IntRect(0, 24, 24, 24));
        sprite.setScale(3.f, 3.f);

        velocity = { 0.f, 0.f };
    }

    void FireEnemy::update(float dt, TileMap& map, GameContext& context)
    {
        if (!isAlive()) return;

        Enemy::update(dt, map, context);

        shootTimer += dt;

        float dx = context.player->getPosition().x - this->getPosition().x;

        int newFacing = facing;

        if (dx > 5.f) newFacing = 1;
        else if (dx < -5.f) newFacing = -1;

        direction = newFacing;

        bool right = (facing == 1);

        if (shootTimer >= shootCooldown)
        {
            shootTimer = 0.f;

            sf::Vector2f pos = sprite.getPosition();

            pos.x += (right ? 40.f : -10.f);
            pos.y += 20.f;

            context.fireballs->push_back(
                std::make_unique<FireBall>(pos, right, Team::Enemy)
            );
        }


        if (newFacing != facing)
        {
            facing = newFacing;

            if (facing == 1)
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

    void FireEnemy::render(sf::RenderWindow& window)
    {
        Enemy::render(window);
    }
}