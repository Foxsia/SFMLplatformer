#pragma once
#include "BrushType.h"
#include <SFML/Graphics.hpp>

namespace fp
{
	class EditorHUD
	{
	public:
		EditorHUD();
		void render(sf::RenderWindow& window, const sf::View& camera, const sf::Font& font, BrushType currentBrush);
	private:
		std::string brushToString(BrushType brush) const;

		sf::Texture tileTexture;
		sf::Texture playerTexture;
		sf::Texture enemyTexture;
		sf::Texture fireEnemyTexture;
		sf::Texture lifeTexture;
		sf::Texture fireTexture;
		sf::Texture goalTexture;
		sf::Texture invulnerabilityTexture;

		sf::Sprite brushSprite;
	};
}

