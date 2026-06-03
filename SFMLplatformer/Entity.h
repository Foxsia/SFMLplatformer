#pragma once

namespace fp
{
    enum EntityState { Alive, Dead };

	class Entity
	{
    protected:
        int health;
        int maxHealth;
        bool alive;
        float damageCooldown;

    public:
        Entity(int hp);

        virtual void takeDamage(int dmg);

        bool isAlive() const { return alive; }

        int getHealth() const { return health; }
	};
}

