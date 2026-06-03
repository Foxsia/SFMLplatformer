#include "Entity.h"
#include <iostream>

namespace fp
{
	Entity::Entity(int hp) : health(hp), maxHealth(hp), alive(true), damageCooldown(0.f)
	{
	}
	void Entity::takeDamage(int dmg)
	{
		if (damageCooldown > 0.f) return;

		health -= dmg;
		std::cout << "HP: " << health << '\n';

		if (health <= 0)
		{
			health = 0;
			alive = false;
			std::cout << "Entity died\n";
		}

		damageCooldown = 0.5f;
	}
}
