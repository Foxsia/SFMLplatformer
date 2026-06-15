#include "HUD.h"
#include "Player.h"

namespace fp
{
	HUD::HUD(sf::Font& font)
	{
		scoreText.setFont(font);
		scoreText.setCharacterSize(24);
		scoreText.setFillColor(sf::Color::White);

		scoreText.setOutlineColor(sf::Color::Black);
		scoreText.setOutlineThickness(2.f);

		healthLivesText.setFont(font);
		healthLivesText.setCharacterSize(24);
		healthLivesText.setFillColor(sf::Color::White);

		healthLivesText.setOutlineColor(sf::Color::Black);
		healthLivesText.setOutlineThickness(2.f);
	}
	void HUD::update(const Player& player)
	{
		scoreText.setString("Score: " + std::to_string(player.getScore()));
		healthLivesText.setString("Health: " + std::to_string(player.getHealth()) + "\nLives: " + std::to_string(player.getLives()));
	}
	void HUD::render(sf::RenderWindow& window)
	{
		const sf::View& view = window.getView();

		scoreText.setPosition(
			view.getCenter().x - view.getSize().x / 2.f + 20.f,
			view.getCenter().y - view.getSize().y / 2.f + 20.f
		);

		window.draw(scoreText);

		healthLivesText.setPosition(
			view.getCenter().x + view.getSize().x / 2.f - healthLivesText.getLocalBounds().width - 20.f,
			view.getCenter().y - view.getSize().y / 2.f + 20.f
		);

		window.draw(healthLivesText);
	}
}
