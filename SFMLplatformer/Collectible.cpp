#include "Collectible.h"

namespace fp
{
	void Collectible::render(sf::RenderTarget& target)
	{
		if (!collected) target.draw(sprite);
	}
	void Collectible::setPosition(float x, float y)
	{
		sprite.setPosition(x, y);
	}
	sf::Vector2f Collectible::getPosition()
	{
		return sprite.getPosition();
	}
}