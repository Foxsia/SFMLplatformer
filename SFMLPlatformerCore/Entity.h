#pragma once
#include <SFML/Graphics.hpp>

namespace fp
{
	class Entity
	{
    protected:
        int health;
        int lives;
        int maxHealth;
        int startLives;
        bool alive;
        float damageCooldown;

        bool damageFlash = false;
        sf::Clock damageFlashClock;

        virtual void setSpriteColor(const sf::Color& color) = 0;
    public:
        Entity(int hp, int lives);

        virtual void takeDamage(int dmg);

        bool isAlive() const { return alive; }

        int getHealth() const { return health; }
        int getLives() const { return lives; }
        int getStartLives() const { return startLives; }
        int getMaxHealth() const { return maxHealth; }

        void heal();
        void addLife(int amount);
        void loseLife(int amount);

        void updateDamageFlash();
	};
}

