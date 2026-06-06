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
		sf::Vector2f move = velocity * dt;
		if (!forward) move *= -1.f;

		for (auto tile : tiles)
		{
			tile->move(move);
		}

		if (!tiles.empty())
		{
			float x = tiles[0]->getPosition().x;

			if (x > 200.f) forward = false;
			if (x < 100.f) forward = true;
		}
	}
	const std::vector<Tile*>& MovingPlatform::getTiles() const
	{
		return tiles;
	}
}
