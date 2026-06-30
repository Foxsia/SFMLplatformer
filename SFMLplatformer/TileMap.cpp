#include "TileMap.h"
#include "Portal.h"
#include <fstream>

namespace fp
{
	namespace
	{
		const unsigned TILE_TEXTURE_X = 0;
		const unsigned TILE_TEXTURE_Y = 0;


		const unsigned MOVING_TILE_TEXTURE_X = 128;
		const unsigned MOVING_TILE_TEXTURE_Y = 0;
		const unsigned SPIKE_TEXTURE_X = 128;
		const unsigned SPIKE_TEXTURE_Y = 288;
	}

	TileMap::TileMap()
	{
		tileSheet = nullptr;
		tileSize = 0;
	}

	TileMap::TileMap(unsigned width, unsigned height, sf::Texture* tile_sheet, unsigned tile_size)
	{
		tileSheet = tile_sheet;
		tileSize = tile_size;

		tiles.resize(width);
		for (int i = 0; i < tiles.size(); i++)
		{
			tiles[i].resize(height);
		}
	}

	Tile* TileMap::getTile(unsigned x, unsigned y)
	{
		if (x < tiles.size())
		{
			if (y < tiles[x].size())
			{
				return tiles[x][y].get();
			}
		}

		return nullptr;
	}

	sf::Vector2f TileMap::getWorldSize() const
	{
		return {
			static_cast<float>(tiles.size()) * tileSize,
			static_cast<float>(tiles[0].size()) * tileSize
		};
	}

	void TileMap::addTile(unsigned x, unsigned y)
	{
		if (x < tiles.size() && x >= 0)
		{
			if (y < tiles[x].size() && y >= 0)
			{
				if (tiles[x][y] == nullptr)
				{
					tiles[x][y] = std::make_unique<Tile>(x, y, tileSheet, sf::IntRect(TILE_TEXTURE_X, TILE_TEXTURE_Y, tileSize, tileSize), TileType::Static);
				}
			}
		}
	}

	void TileMap::removeTile(unsigned x, unsigned y)
	{
		if (x < tiles.size())
		{
			if (y < tiles[x].size())
			{
				if (tiles[x][y] != nullptr)
				{
					tiles[x][y].reset();
				}
			}
		}
	}

	void TileMap::addMovingTile(unsigned x, unsigned y)
	{
		if (x < tiles.size() && x >= 0)
		{
			if (y < tiles[x].size() && y >= 0)
			{
				if (tiles[x][y] == nullptr)
				{
					tiles[x][y] = std::make_unique<Tile>(x, y, tileSheet, sf::IntRect(MOVING_TILE_TEXTURE_X, MOVING_TILE_TEXTURE_Y, tileSize, tileSize), TileType::Moving);
				}
			}
		}
	}

	void TileMap::addSpikeTile(unsigned x, unsigned y)
	{
		if (tiles[x][y] == nullptr)
		{
			tiles[x][y] = std::make_unique<Tile>( x, y, tileSheet, sf::IntRect(SPIKE_TEXTURE_X, SPIKE_TEXTURE_Y, tileSize, tileSize), TileType::Spike);
		}
	}

	void TileMap::clear()
	{
		for (auto& col : tiles)
		{
			for (auto& tile : col)
			{
				tile.reset();
			}
		}
	}

	void TileMap::render(sf::RenderTarget& target)
	{
		sf::Vector2f worldSize = getWorldSize();

		backgroundSprite.setTextureRect(
			sf::IntRect(0, 0,
				static_cast<int>(worldSize.x),
				static_cast<int>(backgroundTexture.getSize().y)
			)
		);

		target.draw(backgroundSprite);

		for (int i = 0; i < tiles.size(); i++)
		{
			for (int k = 0; k < tiles[i].size(); k++)
			{
				if (tiles[i][k] != nullptr) tiles[i][k]->render(target);
			}
		}
	}

	void TileMap::update(float dt)
	{
		for (auto& col : tiles)
		{
			for (auto& tile : col)
			{
				if (tile)
					tile->update(dt);
			}
		}
	}

	void TileMap::saveToFile(const std::string& filename, const std::vector<Enemy*>& enemies, const std::vector<Collectible*>& collectibles)
	{
		std::ofstream file(filename);
		if (!file.is_open()) return;

		std::vector<Portal*> portals;

		file << playerSpawn.x << " " << playerSpawn.y << "\n";

		for (size_t y = 0; y < tiles[0].size(); y++)
		{
			for (size_t x = 0; x < tiles.size(); x++)
			{
				int value = 0;

				if (tiles[x][y] != nullptr)
				{
					if (tiles[x][y]->getType() == TileType::Moving)
					{
						value = 4;
					}
					else if(tiles[x][y]->getType() == TileType::Static)
					{
						value = 1;
					}
					else
					{
						value = 9;
					}
				}

				for (auto enemy : enemies)
				{
					sf::Vector2f pos = enemy->getPosition();

					unsigned enemyX =
						static_cast<unsigned>(pos.x) / tileSize;

					unsigned enemyY =
						static_cast<unsigned>(pos.y) / tileSize;

					if (enemyX == x && enemyY == y)
					{
						value = static_cast<int>(enemy->getType());;
					}
				}

				for (auto collectible : collectibles)
				{
					sf::Vector2f pos = collectible->getPosition();

					unsigned collectibleX =
						static_cast<unsigned>(pos.x) / tileSize;

					unsigned collectibleY =
						static_cast<unsigned>(pos.y) / tileSize;

					if (collectibleX == x && collectibleY == y)
					{
						if (dynamic_cast<LifeFruit*>(collectible))
						{
							value = 3;
						}
						else if (dynamic_cast<Goal*>(collectible))
						{
							value = 5;
						}
						else if (dynamic_cast<FireFruit*>(collectible))
						{
							value = 6;
						}
						else if (dynamic_cast<InvulnerabilityFruit*>(collectible))
						{
							value = 7;
						}
						else if (auto portal = dynamic_cast<Portal*>(collectible))
						{
							portals.push_back(portal);
						}
					}
				}

				file << value << " ";
			}

			file << "\n";
		}

		file << "PORTALS\n";
		file << portals.size() << "\n";

		for (auto portal : portals)
		{
			unsigned x = static_cast<unsigned>(portal->getPosition().x) / tileSize;

			unsigned y = static_cast<unsigned>(portal->getPosition().y) / tileSize;

			file << x << " " << y << " " << portal->getPairId() << "\n";
		}
	}
	void TileMap::loadBackground(const std::string& path)
	{
		backgroundTexture.loadFromFile(path);

		backgroundTexture.setRepeated(true);

		backgroundSprite.setTexture(backgroundTexture);

		backgroundSprite.setPosition(0.f, 0.f);
	}
	void TileMap::setPlayerSpawn(float x, float y)
	{
		playerSpawn = { x, y };
		hasPlayerSpawn = true;
	}
	void TileMap::removePlayerSpawn()
	{
		hasPlayerSpawn = false;
	}
	bool TileMap::hasSpawn() const
	{
		return hasPlayerSpawn;
	}
	const sf::Vector2f& TileMap::getPlayerSpawn() const
	{
		return playerSpawn;
	}
}
