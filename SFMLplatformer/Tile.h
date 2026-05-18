#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Tile
{
private:
	sf::Sprite sprite;
	static constexpr unsigned size = 32;
public:
	Tile(unsigned grid_pos_x, unsigned grid_pos_y, sf::Texture* texture_sheet, sf::IntRect texture_rect);

	const sf::FloatRect getGlobalBounds() const;

	const sf::FloatRect getHitbox() const;

	static constexpr unsigned getSize() { return size; }

	void render(sf::RenderTarget& target);
};

