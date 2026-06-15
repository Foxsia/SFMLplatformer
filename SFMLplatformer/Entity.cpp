#include "Entity.h"
#include <iostream>

namespace fp
{
	Entity::Entity(int hp, int lives) : health(hp), maxHealth(hp), alive(true), damageCooldown(0.f), lives(lives), startLives(lives)
	{
	}
	void Entity::takeDamage(int dmg)
	{
		if (damageCooldown > 0.f) return;

		health -= dmg;

		if (health <= 0)
		{
			lives--;
			health = maxHealth;
		}

		lives <= 0 ? alive = false : alive = true;

		damageCooldown = 1.f;
	}
	void Entity::heal()
	{
		health = maxHealth;
		lives = startLives;
	}
	void Entity::addLife(int amount)
	{
		lives += amount;
	}
	void Entity::loseLife(int amount)
	{
		lives -= amount;
		health = maxHealth;
		lives <= 0 ? alive = false : alive = true;
	}
}
