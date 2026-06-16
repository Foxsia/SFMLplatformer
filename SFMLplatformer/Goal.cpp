#include "Goal.h"
#include "Player.h"

namespace fp
{
	Goal::Goal()
	{
		texture.loadFromFile("assets/end_mushroom.png");

		sprite.setTexture(texture);
	}
	void Goal::onCollect(Player& player)
	{
		reached = true;
		collected = true;
	}
}
