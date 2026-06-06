#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>


namespace fp
{
	enum class TileType
	{
		Static,
		Moving
	};

	class Tile
	{
	public:
		Tile(unsigned grid_pos_x, unsigned grid_pos_y, sf::Texture* texture_sheet, sf::IntRect texture_rect, TileType type = TileType::Static);

		const sf::FloatRect getGlobalBounds() const;

		const sf::FloatRect getHitbox() const;

		static constexpr unsigned getSize() { return size; }

		unsigned getGridX() const { return gridX; }
		unsigned getGridY() const { return gridY; }

		const sf::Vector2f& getPosition() const;

		TileType getType() const { return type; };

		void move(const sf::Vector2f& offset);

		void render(sf::RenderTarget& target);

	private:
		sf::Sprite sprite;
		static constexpr unsigned size = 32;

		unsigned gridX;
		unsigned gridY;

		TileType type;

		static constexpr float hitboxWidth = 28.f;
		static constexpr float hitboxHeight = 32.f;
	};
}

