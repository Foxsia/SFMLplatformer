#pragma once
#include <SFML/Graphics.hpp>

namespace fp
{
	class FireBall
	{
	public:
		FireBall(sf::Vector2f pos, bool right);

		void update(float dt);

		void render(sf::RenderTarget& target);

		sf::FloatRect getBounds() const;

		void destroy() { alive = false; }

		bool isAlive() const { return alive; }

	private:
		sf::CircleShape shape;

		sf::Vector2f velocity;

		bool alive = true;
	};
}

