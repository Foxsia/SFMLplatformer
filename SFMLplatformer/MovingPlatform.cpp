#include "MovingPlatform.h"
#include "Tile.h"

namespace fp
{
	void MovingPlatform::addTile(Tile* tile)
	{
		tiles.push_back(tile);
	}
	void MovingPlatform::update(float dt)
	{
		if (!initialized && !tiles.empty())
		{
			startX = tiles[0]->getPosition().x;
			initialized = true;
		}

		sf::Vector2f move = velocity * dt;
		if (!forward) move *= -1.f;

		for (auto tile : tiles)
		{
			tile->move(move);
		}

		delta = move;

		float currentX = tiles[0]->getPosition().x;

		float distance = currentX - startX;

		if (!tiles.empty())
		{
			if (distance >= 5 * tiles[0]->getSize()) forward = false;
			if (distance <= 0) forward = true;
		}
	}
	const sf::Vector2f& MovingPlatform::getDelta() const
	{
		return delta;
	}
	const std::vector<Tile*>& MovingPlatform::getTiles() const
	{
		return tiles;
	}
}
