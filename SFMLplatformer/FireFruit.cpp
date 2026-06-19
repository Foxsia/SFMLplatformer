#include "FireFruit.h"
#include "Player.h"

namespace fp
{
	namespace
	{
		const int SCORE_COLLECTIBLE = 75;
	}
	FireFruit::FireFruit()
	{
		texture.loadFromFile("assets/fire_fruit.png");

		sprite.setTexture(texture);
	}
	void FireFruit::onCollect(Player& player)
	{
		player.setFirePower(true);
		player.addScore(SCORE_COLLECTIBLE);

		collected = true;
	}
}
