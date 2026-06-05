#include "Collectible.h"

namespace fp
{
	Collectible::Collectible()
	{
		texture.loadFromFile("assets/fruit.png");

		sprite.setTexture(texture);

		collected = false;
	}
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
	void Collectible::collect()
	{
		collected = true;
	}
}