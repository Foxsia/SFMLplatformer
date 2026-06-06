#pragma once
#include <vector>
#include <SFML/System.hpp>

namespace fp
{
	namespace
	{
		const float VELOCITY_X = 50.f;
		const float VELOCITY_Y = 0.f;
	}
	class Tile;

	class MovingPlatform
	{
	public:
		void addTile(Tile* tile);
		void update(float dt);

		const std::vector<Tile*>& getTiles() const;
	private:
		std::vector<Tile*> tiles;

		sf::Vector2f startPos;
		sf::Vector2f endPos;

		sf::Vector2f velocity = { VELOCITY_X, VELOCITY_Y };
		bool forward = true;
	};
}
