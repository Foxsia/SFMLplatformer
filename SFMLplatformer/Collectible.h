#pragma once
#include <SFML/Graphics.hpp>

namespace fp
{
	class Collectible
	{
	public:
		Collectible();

		void render(sf::RenderTarget& target);
		sf::FloatRect getGlobalBounds() const { return sprite.getGlobalBounds(); };

		void setPosition(float x, float y);
		sf::Vector2f getPosition();

		bool isCollected() const { return collected; };

		void collect();
	private:
		sf::Sprite sprite;
		sf::Texture texture;

		bool collected;
	};
}

