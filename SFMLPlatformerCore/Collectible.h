#pragma once
#include <SFML/Graphics.hpp>

namespace fp
{
	class Player;

	class Collectible
	{
	public:

		void render(sf::RenderTarget& target);
		virtual void onCollect(Player& player) = 0;
		sf::FloatRect getGlobalBounds() const { return sprite.getGlobalBounds(); };

		void setPosition(float x, float y);
		sf::Vector2f getPosition();

		bool isCollected() const { return collected; };
		virtual bool isGoal() const { return false; }
	protected:
		sf::Sprite sprite;
		sf::Texture texture;

		bool collected = false;
	};
}

