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

		const sf::Vector2f& getDelta() const;

		const std::vector<Tile*>& getTiles() const;
	private:
		std::vector<Tile*> tiles;

		sf::Vector2f velocity = { VELOCITY_X, VELOCITY_Y };
		bool forward = false;

		sf::Vector2f delta;

		float startX = 0.f;
		bool initialized = false;
	};
}
