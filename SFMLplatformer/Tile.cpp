#include "Tile.h"


namespace fp
{
	namespace
	{
		const float SPIKE_HIDDEN_TIME = 2.0f;
		const float SPIKE_RAISE_TIME = 0.2f;
		const float SPIKE_ACTIVE_TIME = 1.0f;
		const float SPIKE_LOWER_TIME = 0.2f;
	}
	Tile::Tile(unsigned grid_pos_x, unsigned grid_pos_y, sf::Texture* texture_sheet, sf::IntRect texture_rect, TileType type) : type(type), gridX(grid_pos_x), gridY(grid_pos_y)
	{
		sprite.setTexture(*texture_sheet);
		sprite.setTextureRect(texture_rect);
		sprite.setPosition(float(grid_pos_x * size), float(grid_pos_y * size));
	}

	const sf::FloatRect Tile::getGlobalBounds() const
	{
		return sprite.getGlobalBounds();
	}

	const sf::FloatRect Tile::getHitbox() const
	{
		return sf::FloatRect(sprite.getPosition(), sf::Vector2f(hitboxWidth, hitboxHeight));
	}

	const sf::Vector2f& Tile::getPosition() const
	{
		return sprite.getPosition();
	}

	void Tile::move(const sf::Vector2f& offset)
	{
		sprite.move(offset);
	}

	void Tile::render(sf::RenderTarget& target)
	{
		if (type == TileType::Spike)
		{
			if (spikeActive)
			{
				sprite.setTextureRect(sf::IntRect(128, 256, 32, 32));
			}
			else
			{
				sprite.setTextureRect(sf::IntRect(128, 236, 32, 32));
			}
		}
		target.draw(sprite);
	}
	void Tile::update(float dt)
	{
		if (type != TileType::Spike) return;

		spikeTimer += dt;

		if (!spikeActive)
		{
			if (spikeTimer >= SPIKE_HIDDEN_TIME)
			{
				spikeTimer = 0.f;
				spikeActive = true;
				spikeExtending = true;
			}
		}
		else
		{
			if (spikeTimer >= SPIKE_ACTIVE_TIME)
			{
				spikeTimer = 0.f;
				spikeActive = false;
			}
		}
	}
}
