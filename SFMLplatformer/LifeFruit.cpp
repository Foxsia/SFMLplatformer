#include "LifeFruit.h"
#include "Player.h"

namespace fp
{
	namespace
	{
		const int SCORE_COLLECTIBLE = 50;
	}
	LifeFruit::LifeFruit()
	{
		texture.loadFromFile("assets/fruit.png");

		sprite.setTexture(texture);
	}
	void LifeFruit::onCollect(Player& player)
	{
		player.addLife(1);
		player.addScore(SCORE_COLLECTIBLE);

		collected = true;
	}
}
