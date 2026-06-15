#pragma once

#include <SFML/Graphics.hpp>

namespace fp
{
	class Player;

	class HUD
	{
	private:
		sf::Text scoreText;
		sf::Text healthLivesText;

	public:
		HUD(sf::Font& font);

		void update(const Player& player);

		void render(sf::RenderWindow& window);
	};
}