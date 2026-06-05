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

		if (health <= 0)
		{
			health = 0;
			alive = false;
		}

		damageCooldown = 1.f;
	}
	void Entity::heal(int amount)
	{
		health += amount;
		if (health > maxHealth) health = maxHealth;
	}
}
