#pragma once
#include "Enemy.h"

namespace fp
{
	class FireEnemy : public Enemy
	{
	public:
		FireEnemy();

		void update(float dt, TileMap& map, GameContext& context) override;
		void render(sf::RenderWindow&) override;

	private:
		void checkDirectionChange(TileMap& map);
		bool chasingPlayer = false;

		float shootTimer = 0.f;
		float shootCooldown = 2.f;
		int facing = 1;
	};
}

