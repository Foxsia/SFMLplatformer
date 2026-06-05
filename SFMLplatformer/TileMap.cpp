#include "TileMap.h"
#include <fstream>

namespace fp
{
	namespace
	{
		const unsigned TILE_TEXTURE_X = 0;
		const unsigned TILE_TEXTURE_Y = 0;
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
			tiles[i].resize(height, nullptr);
		}
	}

	TileMap::~TileMap()
	{
		for (int i = 0; i < tiles.size(); i++)
		{
			for (int k = 0; k < tiles[i].size(); k++)
			{
				delete tiles[i][k];
				tiles[i][k] = nullptr;
			}
		}
	}

	Tile* TileMap::getTile(unsigned x, unsigned y)
	{
		if (x < tiles.size())
		{
			if (y < tiles[x].size())
			{
				return tiles[x][y];
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
					tiles[x][y] = new Tile(x, y, tileSheet, sf::IntRect(TILE_TEXTURE_X, TILE_TEXTURE_Y, tileSize, tileSize));
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
					delete tiles[x][y];
					tiles[x][y] = nullptr;
				}
			}
		}
	}

	void TileMap::clear()
	{
		for (size_t x = 0; x < tiles.size(); ++x)
		{
			for (size_t y = 0; y < tiles[x].size(); ++y)
			{
				delete tiles[x][y];
				tiles[x][y] = nullptr;
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

	void TileMap::saveToFile(const std::string& filename, const std::vector<Enemy*>& enemies, const std::vector<Collectible*>& collectibles)
	{
		std::ofstream file(filename);
		if (!file.is_open()) return;

		file << playerSpawn.x << " " << playerSpawn.y << "\n";

		for (size_t y = 0; y < tiles[0].size(); y++)
		{
			for (size_t x = 0; x < tiles.size(); x++)
			{
				int value = 0;

				if (tiles[x][y] != nullptr)
				{
					value = 1;
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
						value = 2;
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
						value = 3;
					}
				}

				file << value << " ";
			}

			file << "\n";
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
