#include "Tile.h"
#include "ConfigManager.h"

namespace fp
{
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
		auto& cfg = ConfigManager::get("spikes");
		if (type != TileType::Spike) return;

		spikeTimer += dt;

		if (!spikeActive)
		{
			if (spikeTimer >= cfg.at("hiddenTime").get<float>())
			{
				spikeTimer = 0.f;
				spikeActive = true;
				spikeExtending = true;
			}
		}
		else
		{
			if (spikeTimer >= cfg.at("activeTime").get<float>())
			{
				spikeTimer = 0.f;
				spikeActive = false;
			}
		}
	}
}
